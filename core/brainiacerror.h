// Brainiac is a free and open source tool for the creation of crowd simulation

// Copyright (C) 2013  Dirk Fuchs dirkfux@googlemail.com

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


#ifndef BRAINIACERROR_H
#define BRAINIACERROR_H
#include <QString>

namespace BrainiacError
{
  enum ErrorCode
  {
    OK = 0,
    INTERNAL,
    MEMORY_ALLOCATION_FAILED,
    FILE_NOT_FOUND,
    INVALID_FILE_FORMAT,
    FILE_PARSER_FAILED,
    COULD_NOT_OPEN_FILE,
    COULD_NOT_WRITE_FILE,
    MAX_ERROR_CODE
  };

  ErrorCode getLastErrorCode();
  QString getLastErrorFile();
  int getLastErrorLine();
  QString getLastErrorText();
  void printLastError();
  void setLastError(ErrorCode code, const QString& file, int line, const QString& text = "");

  QString getErrorDescription(ErrorCode code);

  inline QString getLastErrorDescription() {
      return getErrorDescription(getLastErrorCode());
  }
}


#endif // BRAINIACERROR_H
