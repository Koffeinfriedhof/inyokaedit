/**
 * \file CParseTemplates.cpp
 *
 * \section LICENSE
 *
 * Copyright (C) 2011-2014 The InyokaEdit developers
 *
 * This file is part of InyokaEdit.
 *
 * InyokaEdit is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * InyokaEdit is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with InyokaEdit.  If not, see <http://www.gnu.org/licenses/>.
 *
 * \section DESCRIPTION
 * Parsing templates.
 */

#include <QDebug>

#include "./CParseTemplates.h"

CParseTemplates::CParseTemplates(QString sTransTpl,
                                 QStringList sListTplNames,
                                 const QStringList &sListHtmlStart,
                                 const QDir &tmpFileOutputDir,
                                 const QDir &tmpImgDir)
    : m_sTransTpl(sTransTpl),
      m_sListTplNames(sListTplNames) {
    m_pProvTplTarser = new CProvisionalTplParser(sListHtmlStart,
                                                 tmpFileOutputDir,
                                                 tmpImgDir);
}

// ----------------------------------------------------------------------------
// ----------------------------------------------------------------------------

void CParseTemplates::startParsing(QTextDocument *pRawDoc,
                                   const QString &sCurrentFile) {
    m_sCurrentFile = sCurrentFile;

    QStringList sListTplRegExp;
    sListTplRegExp << "\\{\\{\\{#!" + m_sTransTpl + " .+\\}\\}\\}"
                   << "\\[\\[" + m_sTransTpl + "\\s*\\(.+\\)\\]\\]";
    QString sMyDoc = pRawDoc->toPlainText();
    QString sMacro;
    QString sBackupMacro;
    QStringList sListArguments;
    int nPos;

    for (int k = 0; k < sListTplRegExp.size(); k++) {
        QRegExp findTemplate(sListTplRegExp[k], Qt::CaseInsensitive);
        findTemplate.setMinimal(true);
        nPos = 0;

        while ((nPos = findTemplate.indexIn(sMyDoc, nPos)) != -1) {
            sMacro = findTemplate.cap(0);
            sBackupMacro = sMacro;
            if (sMacro.startsWith("[[" + m_sTransTpl, Qt::CaseInsensitive)) {
                // Step needed because of possible spaces
                sMacro.remove("[[" + m_sTransTpl, Qt::CaseInsensitive);
                sMacro = sMacro.trimmed();
            }
            sListArguments.clear();

            // Check if macro exists
            for (int i = 0; i < m_sListTplNames.size(); i++) {
                if (sMacro.startsWith("(" + m_sListTplNames[i],
                                      Qt::CaseInsensitive)) {
                    sMacro.remove(0, 1);  // Remove (
                    sMacro.remove("\n)]]");
                    sMacro.remove(")]]");

                    // Extract arguments
                    // Split by ',' but don't split quoted strings with comma
                    QStringList tmpList = sMacro.split(QRegExp("\""));
                    bool bInside = false;
                    foreach (QString s, tmpList) {
                        if (bInside) {
                            // If 's' is inside quotes, get the whole string
                            sListArguments.append(s);
                        } else {
                            // If 's' is outside quotes, get the splitted string
                            sListArguments.append(s.split(QRegExp(",+"),
                                                          QString::SkipEmptyParts));
                        }
                        bInside = !bInside;
                    }
                    sListArguments.removeAll(" ");

                    // In addition to ',' arguments can be separated by '\n'...
                    for (int i = 0; i < sListArguments.size(); i++) {
                        if (sListArguments[i].contains("\n")) {
                            QString sTmp = sListArguments[i];
                            QStringList tmpArgs;
                            tmpArgs << sTmp.split(QRegExp("\\n"),
                                                  QString::SkipEmptyParts);
                            for (int j = 0; j < tmpArgs.size(); j++) {
                                sListArguments.insert(i + j + 1, tmpArgs[j]);
                            }
                            sListArguments.removeAt(i);
                        }
                    }
                } else if (sMacro.startsWith("{{{#!" +  m_sTransTpl + " "
                                             + m_sListTplNames[i],
                                             Qt::CaseInsensitive)) {
                    sMacro.remove("{{{#!" + m_sTransTpl + " ",
                                  Qt::CaseInsensitive);
                    sMacro.remove("\n\\}}}");
                    sMacro.remove("\\}}}");
                    sMacro.remove("\n}}}");
                    sMacro.remove("}}}");
                    sListArguments.clear();

                    // Extract arguments
                    sListArguments = sMacro.split(QRegExp("\\n"));

                    if (sListArguments.size() > 0) {
                        // Split by ' ' but don't split quoted strings with space
                        QStringList sList;
                        QStringList sList2 = sListArguments[0].split(QRegExp("\""));
                        bool bInside = false;
                        foreach (QString s, sList2) {
                            if (bInside) {
                                // If 's' is inside quotes, get the whole string
                                sList.append(s);
                            } else {
                                // If 's' is outside quotes, get the splitted string
                                sList.append(s.split(QRegExp("\\s+"),
                                                              QString::SkipEmptyParts));
                            }
                            bInside = !bInside;
                        }
                        if (sList.size() > 1) {
                            sListArguments.removeFirst();
                            for (int i = sList.size() - 1; i >= 0; i--) {
                                sListArguments.push_front(sList[i]);
                            }
                        }
                    }
                }
            }

            for (int j = 0; j < sListArguments.size(); j++) {
                sListArguments[j] = sListArguments[j].trimmed();
            }

            qDebug() << "TPL:" << sListArguments;
            sMacro = m_pProvTplTarser->parseTpl(sListArguments, m_sCurrentFile);
            if (sMacro.isEmpty()) {
                sMacro = sBackupMacro;
            }
            sMyDoc.replace(nPos, findTemplate.matchedLength(), sMacro);

            // Go on with new start position
            nPos += sMacro.length();
        }
    }

    pRawDoc->setPlainText(sMyDoc);
}
