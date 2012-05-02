/* --------------------------------------------------------------------------

   libcoverart - Client library to access CoverArtArchive

   Copyright (C) 2012 Andrew Hawkins

   This file is part of libcoverart.

   This library is free software; you can redistribute it and/or
   modify it under the terms of v2 of the GNU Lesser General Public
   License as published by the Free Software Foundation.

   libcoverart is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.

     $Id$

----------------------------------------------------------------------------*/

#ifndef _COVERARTARCHIVE_IMAGE_
#define _COVERARTARCHIVE_IMAGE_

#include <iostream>

#include <jansson.h>

namespace CoverArtArchive
{
	class CImagePrivate;
	class CThumbnails;
	class CTypes;

	class CImage
	{
		public:
			CImage(json_t *Root=0);
			CImage(const CImage& Other);
			CImage& operator =(const CImage& Other);
			virtual ~CImage();

			bool Approved() const;
			bool Back() const;
			std::string Comment() const;
			int Edit() const;
			bool Front() const;
			std::string ID() const;
			std::string Image() const;
			CoverArtArchive::CThumbnails *Thumbnails() const;
			CoverArtArchive::CTypes *Types() const;

		private:
			CImagePrivate * const m_d;

			void Cleanup();
	};
}

std::ostream& operator << (std::ostream& os, const CoverArtArchive::CImage& Image);

#endif