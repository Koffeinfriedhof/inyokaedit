#  This file is part of InyokaEdit.
#  Copyright (C) 2011-2015 The InyokaEdit developers
#
#  InyokaEdit is free software: you can redistribute it and/or modify
#  it under the terms of the GNU General Public License as published by
#  the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  InyokaEdit is distributed in the hope that it will be useful,
#  but WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#  GNU General Public License for more details.
#
#  You should have received a copy of the GNU General Public License
#  along with InyokaEdit.  If not, see <http://www.gnu.org/licenses/>.

INCLUDEPATH += $$PWD
DEPENDPATH  += $$PWD

HEADERS     += $$PWD/CParser.h \
               $$PWD/CParseImgMap.h \
               $$PWD/CParseLinks.h \
               $$PWD/CParseTemplates.h \
               $$PWD/CProvisionalTplParser.h

SOURCES     += $$PWD/CParser.cpp \
               $$PWD/CParseImgMap.cpp \
               $$PWD/CParseLinks.cpp \
               $$PWD/CParseTemplates.cpp \
               $$PWD/CProvisionalTplParser.cpp
