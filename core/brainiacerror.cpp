// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2012  Dirk Fuchs dirkfux@googlemail.com

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "brainiacerror.h"
#include <iostream>
namespace
{
    BrainiacError::ErrorCode m_lastErrorCode = BrainiacError::OK;
    QString m_strLastErrorFile;
    int m_lastErrorLine = -1;
    QString m_strLastErrorText;
}


QString BrainiacError::getErrorDescription(ErrorCode code)
{
  switch(code)
  {
    case OK:                         return "No error found";
    case INTERNAL:                   return "Internal error";
    case MEMORY_ALLOCATION_FAILED:   return "Memory allocation failed";
    case FILE_NOT_FOUND:             return "File not found";
    case INVALID_FILE_FORMAT:        return "Invalid file format";
    case FILE_PARSER_FAILED:         return "Parser failed to process file";
    default:                         return "Unknown error";
  }
}

BrainiacError::ErrorCode BrainiacError::getLastErrorCode()
{
  return m_lastErrorCode;
}

QString BrainiacError::getLastErrorFile()
{
  return m_strLastErrorFile;
}


int BrainiacError::getLastErrorLine()
{
  return m_lastErrorLine;
}

QString BrainiacError::getLastErrorText()
{
  return m_strLastErrorText;
}


void BrainiacError::printLastError()
{
  std::cout << "Brainiac : " << getLastErrorDescription().toStdString();

  // only print supplementary information if there is some
  if(m_strLastErrorText.length() > 0)
  {
    std::cout << " '" << m_strLastErrorText.toStdString() << "'";
  }

  std::cout << " in " << m_strLastErrorFile.toStdString() << "(" << m_lastErrorLine << ")" << std::endl;
}

void BrainiacError::setLastError(ErrorCode code, const QString& file, int line, const QString& text)
{
  if(code >= MAX_ERROR_CODE) code = INTERNAL;

  m_lastErrorCode = code;
  m_strLastErrorFile = file;
  m_lastErrorLine = line;
  m_strLastErrorText = text;
}
