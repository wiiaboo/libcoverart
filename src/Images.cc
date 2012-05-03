/* --------------------------------------------------------------------------

   libcoverart - Client library to access MusicBrainz

   Copyright (C) 2011 Andrew Hawkins

   This file is part of libcoverart.

   This library is free software; you can redistribute it and/or
   modify it under the terms of v2 of the GNU Lesser General Public
   License as published by the Free Software Foundation.

   libcoverart is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this library.  If not, see <http://www.gnu.org/licenses/>.

     $Id$

----------------------------------------------------------------------------*/

#include "config.h"
#include "coverart/defines.h"

#include <vector>

#include <jansson.h>

#include "coverart/Images.h"
#include "coverart/Image.h"

class CoverArtArchive::CImagesPrivate
{
	public:
		CImagesPrivate()
		{
		};

		std::vector<CoverArtArchive::CImage *> m_Images;
};

CoverArtArchive::CImages::CImages(json_t *Root)
:	m_d(new CImagesPrivate)
{
	if (Root && json_is_array(Root))
	{
		for (size_t count=0;count<json_array_size(Root);count++)
		{
			json_t *Image=json_array_get(Root,count);
			m_d->m_Images.push_back(new CImage(Image));
		}
	}
}

CoverArtArchive::CImages::CImages(const CImages& Other)
:	m_d(new CImagesPrivate)
{
	*this=Other;
}

CoverArtArchive::CImages& CoverArtArchive::CImages::operator =(const CImages& Other)
{
	if (this!=&Other)
	{
		Cleanup();

		std::vector<CImage *>::const_iterator ThisImage=Other.m_d->m_Images.begin();
		while (ThisImage!=Other.m_d->m_Images.end())
		{
			CImage *Image=(*ThisImage);
			m_d->m_Images.push_back(new CImage(*Image));
			++ThisImage;
		}
	}

	return *this;
}

CoverArtArchive::CImages::~CImages()
{
	Cleanup();

	delete m_d;
}

void CoverArtArchive::CImages::Cleanup()
{
	while (!m_d->m_Images.empty())
	{
		delete m_d->m_Images.back();
		m_d->m_Images.pop_back();
	}
}

int CoverArtArchive::CImages::NumItems() const
{
	return m_d->m_Images.size();
}

CoverArtArchive::CImage *CoverArtArchive::CImages::Item(int Item) const
{
	return m_d->m_Images[Item];
}

std::ostream& operator << (std::ostream& os, const CoverArtArchive::CImages& Images)
{
	for (int count=0;count<Images.NumItems();count++)
		os << *Images.Item(count) << std::endl;

	return os;
}


