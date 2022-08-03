/*****************************************************************************
 *
 * buffered_file.h
 *   Memory file buffer, used for recording in memory in context of SV.
 *
 *****************************************************************************
 *
 * Copyright 2013-2022 Sighthound, Inc.
 *
 * Licensed under the GNU GPLv3 license found at
 * https://www.gnu.org/licenses/gpl-3.0.txt
 *
 * Alternative licensing available from Sighthound, Inc.
 * by emailing opensource@sighthound.com
 *
 * This file is part of the Sighthound Video project which can be found at
 * https://github.url/thing
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; using version 3 of the License.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02111, USA.
 *
 *****************************************************************************/

#include <memory>
#include <vector>
#include <fstream>
#include <string>

#include "sv_os.h"

class IBufferedFile
{
public:

    virtual ~IBufferedFile() {};

    // access to opaque value settable by the consumer
    virtual void                        setOpaque(void* pOpaque) = 0;
    virtual void*                       getOpaque() const = 0;

    // get file name associated with this object
    virtual const std::string&          getName() const = 0;

    // seek to a position in the file, but not beyond EOF
    virtual int64_t                     seek(int64_t pos, int dir) = 0;

    // write to the current position in the buffer
    virtual bool                        write(char* data, size_t size) = 0;

    // save the contents to file
    virtual bool                        save(const char* sFilename) = 0;

    // save the contents to stream
    virtual bool                        save(std::ostream& os) = 0;
};

SVCORE_API IBufferedFile* _CreateBufferedFile( const std::string& name = std::string(""),
                int bufferSize = 1024*1024,
                int maxSize = 512*1024*1024 );
