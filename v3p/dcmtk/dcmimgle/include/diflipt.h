/*
 *
 *  Copyright (C) 1996-2001, OFFIS
 *
 *  This software and supporting documentation were developed by
 *
 *    Kuratorium OFFIS e.V.
 *    Healthcare Information and Communication Systems
 *    Escherweg 2
 *    D-26121 Oldenburg, Germany
 *
 *  THIS SOFTWARE IS MADE AVAILABLE,  AS IS,  AND OFFIS MAKES NO  WARRANTY
 *  REGARDING  THE  SOFTWARE,  ITS  PERFORMANCE,  ITS  MERCHANTABILITY  OR
 *  FITNESS FOR ANY PARTICULAR USE, FREEDOM FROM ANY COMPUTER DISEASES  OR
 *  ITS CONFORMITY TO ANY SPECIFICATION. THE ENTIRE RISK AS TO QUALITY AND
 *  PERFORMANCE OF THE SOFTWARE IS WITH THE USER.
 *
 *  Module:  dcmimgle
 *
 *  Author:  Joerg Riesmeier
 *
 *  Purpose: DicomFlipTemplate (Header)
 *
 *  Last Update:      $Author: amithaperera $
 *  Update Date:      $Date: 2004/01/14 04:01:10 $
 *  Source File:      Source
 *  CVS/RCS Revision: $Revision: 1.1 $
 *  Status:           $State: Exp $
 *
 *  CVS/RCS Log at end of file
 *
 */


#ifndef __DIFLIPT_H
#define __DIFLIPT_H

#include "osconfig.h"
#include "dctypes.h"

#include "dipixel.h"
#include "ditranst.h"


/*---------------------*
 *  class declaration  *
 *---------------------*/

/** Template class to flip images (on pixel data level).
 *  horizontally and vertically
 */
template<class T>
class DiFlipTemplate
  : public DiTransTemplate<T>
{

 public:

    /** constructor.
     *  This method is used to flip an image and store the result in the same storage area.
     *
     ** @param  pixel    pointer to object where the pixel data are stored
     *  @param  columns  width of the image
     *  @param  rows     height of the image
     *  @param  frames   number of frames
     *  @param  horz     flags indicating whether to flip horizontally or not
     *  @param  vert     flags indicating whether to flip vertically or not
     */
    DiFlipTemplate(DiPixel *pixel,
                   const Uint16 columns,
                   const Uint16 rows,
                   const Uint32 frames,
                   const int horz,
                   const int vert)
      : DiTransTemplate<T>(0, columns, rows, columns, rows, frames)
    {
        if (pixel != NULL)
        {
            Planes = pixel->getPlanes();
            if ((pixel->getCount() > 0) && (Planes > 0) &&
                (pixel->getCount() == (unsigned long)columns * (unsigned long)rows * frames))
            {
                if (horz && vert)
                    flipHorzVert((T **)pixel->getDataPtr());
                else if (horz)
                    flipHorz((T **)pixel->getDataPtr());
                else if (vert)
                    flipVert((T **)(pixel->getDataPtr()));
            } else {
                if (DicomImageClass::checkDebugLevel(DicomImageClass::DL_Warnings))
                {
                   ofConsole.lockCerr() << "WARNING: could not flip image ... corrupted data." << endl;
                   ofConsole.unlockCerr();
                }
            }
        }
    }

    /** constructor.
     *  This method is used to perform only the preparation and to start flipping later with method 'flipData()'
     *
     ** @param  planes   number of planes (1 or 3)
     *  @param  columns  width of the image
     *  @param  rows     height of the image
     *  @param  frames   number of frames
     */
    DiFlipTemplate(const int planes,
                   const Uint16 columns,
                   const Uint16 rows,
                   const Uint32 frames)
      : DiTransTemplate<T>(planes, columns, rows, columns, rows, frames)
    {
    }

    /** destructor
     */
    virtual ~DiFlipTemplate()
    {
    }

    /** choose algorithm depending on flipping mode
     *
     ** @param  src   array of pointers to source image pixels
     *  @param  dest  array of pointers to destination image pixels
     *  @param  horz  flags indicating whether to flip horizontally or not
     *  @param  vert  flags indicating whether to flip vertically or not
     */
    inline void flipData(const T *src[],
                         T *dest[],
                         const int horz,
                         const int vert)
    {
        if ((src != NULL) && (dest != NULL))
        {
            if (horz && vert)
                flipHorzVert(src, dest);
            else if (horz)
                flipHorz(src, dest);
            else if (vert)
                flipVert(src, dest);
            else
                copyPixel(src, dest);
        }
    }


 protected:

   /** flip source image horizontally and store result in destination image
    *
    ** @param  src   array of pointers to source image pixels
    *  @param  dest  array of pointers to destination image pixels
    */
    inline void flipHorz(const T *src[],
                         T *dest[])
    {
        if ((src != NULL) && (dest != NULL))
        {
            register Uint16 x;
            register Uint16 y;
            register const T *p;
            register T *q;
            register T *r;
            for (int j = 0; j < Planes; j++)
            {
                p = src[j];
                r = dest[j];
                for (Uint32 f = Frames; f != 0; f--)
                {               
                    for (y = Src_Y; y != 0; y--)
                    {
                        q = r + Dest_X;
                        for (x = Src_X; x != 0; x--)
                            *--q = *p++;
                        r += Dest_X;
                    }
                }
            }
        }
    }
 
   /** flip source image vertically and store result in destination image
    *
    ** @param  src   array of pointers to source image pixels
    *  @param  dest  array of pointers to destination image pixels
    */
    inline void flipVert(const T *src[],
                         T *dest[])
    {
        if ((src != NULL) && (dest != NULL))
        {
            register Uint16 x;
            register Uint16 y;
            register const T *p;
            register T *q;
            register T *r;
            const unsigned long count = (unsigned long)Dest_X * (unsigned long)Dest_Y;
            for (int j = 0; j < Planes; j++)
            {
                p = src[j];
                r = dest[j];
                for (Uint32 f = Frames; f != 0; f--)
                {          
                    r += count;     
                    for (y = Src_Y; y != 0; y--)
                    {
                        q = r - Dest_X;
                        for (x = Src_X; x != 0; x--)
                            *q++ = *p++;
                        r -= Dest_X;
                    }
                    r += count;
                }
            }
        }
    }

   /** flip source image horizontally and vertically and store result in destination image
    *
    ** @param  src   array of pointers to source image pixels
    *  @param  dest  array of pointers to destination image pixels
    */
    inline void flipHorzVert(const T *src[],
                             T *dest[])
    {
       if ((src != NULL) && (dest != NULL))
       {
            register unsigned long i;
            register const T *p;
            register T *q;
            const unsigned long count = (unsigned long)Dest_X * (unsigned long)Dest_Y;
            for (int j = 0; j < Planes; j++)
            {
                p = src[j];
                q = dest[j];
                for (Uint32 f = Frames; f != 0; f--)
                {
                    q += count;
                    for (i = count; i != 0; i--)
                        *--q = *p++;
                    q += count;
                }
            }
        }
    }

 private:

   /** flip image horizontally and store result in the same storage area
    *
    ** @param  data  array of pointers to source/destination image pixels
    */
    inline void flipHorz(T *data[])
    {
        register Uint16 x;
        register Uint16 y;
        register T *p;
        register T *q;
        register T t;
        T *r;
        for (int j = 0; j < Planes; j++)
        {
            r = data[j];
            for (Uint32 f = Frames; f != 0; f--)
            {               
                for (y = Src_Y; y != 0; y--)
                {
                    p = r;
                    r += Dest_X;
                    q = r;
                    for (x = Src_X / 2; x != 0; x--)
                    {
                        t = *p;
                        *p++ = *--q;
                        *q = t;
                    }
                }
            }
        }
    }

   /** flip image vertically and store result in the same storage area
    *
    ** @param  data  array of pointers to source/destination image pixels
    */
    inline void flipVert(T *data[])
    {
        register Uint16 x;
        register Uint16 y;
        register T *p;
        register T *q;
        register T *r;
        register T t;
        T *s;
        const unsigned long count = (unsigned long)Dest_X * (unsigned long)Dest_Y;
        for (int j = 0; j < Planes; j++)
        {
            s = data[j];
            for (Uint32 f = Frames; f != 0; f--)
            {              
                p = s; 
                s += count;
                r = s; 
                for (y = Src_Y / 2; y != 0; y--)
                {
                    r -= Dest_X;
                    q = r;
                    for (x = Src_X; x != 0; x--)
                    {
                        t = *p;
                        *p++ = *q;
                        *q++ = t;
                    }
                }
            }
        }
    }

   /** flip image horizontally and vertically and store result in the same storage area
    *
    ** @param  data  array of pointers to source/destination image pixels
    */
    inline void flipHorzVert(T *data[])
    {
        register unsigned long i;
        register T *p;
        register T *q;
        register T t;
        T *s;
        const unsigned long count = (unsigned long)Dest_X * (unsigned long)Dest_Y;
        for (int j = 0; j < Planes; j++)
        {
            s = data[j];
            for (Uint32 f = Frames; f != 0; f--)
            {               
                p = s;
                q = s + count;
                for (i = count / 2; i != 0; i--)
                {
                    t = *p;
                    *p++ = *--q;
                    *q = t;
                }
            }
        }
    }
};


#endif
                        

/*
 *
 * CVS/RCS Log:
 * Log: diflipt.h
 * Revision 1.12  2001/06/01 15:49:41  meichel
 * Updated copyright header
 *
 * Revision 1.11  2000/09/12 10:04:44  joergr
 * Corrected bug: wrong parameter for attribute search routine led to crashes
 * when multiple pixel data attributes were contained in the dataset (e.g.
 * IconImageSequence). Added new checking routines to avoid crashes when
 * processing corrupted image data.
 *
 * Revision 1.10  2000/03/08 16:24:15  meichel
 * Updated copyright header.
 *
 * Revision 1.9  2000/03/02 12:51:36  joergr
 * Rewrote variable initialization in class contructors to avoid warnings
 * reported on Irix.
 *
 * Revision 1.8  1999/09/17 12:10:55  joergr
 * Added/changed/completed DOC++ style comments in the header files.
 * Enhanced efficiency of some "for" loops.
 *
 * Revision 1.7  1999/05/03 11:09:28  joergr
 * Minor code purifications to keep Sun CC 2.0.1 quiet.
 *
 * Revision 1.6  1999/04/28 14:46:54  joergr
 * Removed debug code.
 *
 * Revision 1.5  1999/03/24 17:20:00  joergr
 * Added/Modified comments and formatting.
 *
 * Revision 1.4  1999/02/03 17:01:16  joergr
 * Removed some debug code.
 *
 * Revision 1.3  1999/01/20 14:59:05  joergr
 * Added debug code to measure time of some routines.
 *
 * Revision 1.2  1998/12/16 16:27:54  joergr
 * Added additional case to copy pixels.
 *
 * Revision 1.1  1998/11/27 14:57:46  joergr
 * Added copyright message.
 * Added methods and classes for flipping and rotating, changed for
 * scaling and clipping.
 *
 *
 */
