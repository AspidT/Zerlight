//
// Created by tatiana on 27.09.18.
//

#include "SDLGraphics.h"
#include "SDLPixel.h"
#include "SDLImage.h"
#include "SDLDeleters.h"
#include "guichan/gui/Exception.h"

#include <SDL2/SDL_surface.h>
#include <SDL2/SDL_render.h>
#include <SDL2/SDL_endian.h>

namespace sdl_interface
{
    SDLGraphics::SDLGraphics()
    {
        mAlpha = false;
        mTarget = nullptr;
    }

    void clearTransparentSurface(std::shared_ptr<SDL_Surface> s)
    {
        SDL_FillRect(s.get(), nullptr, SDL_MapRGBA(s->format, 0, 0, 0, 0));
    }

    void SDLGraphics::beginDraw()
    {
        gui::Rectangle area;
        area.x = 0;
        area.y = 0;
        area.width = mTarget->w;
        area.height = mTarget->h;
        pushClipArea(area);

        clearTransparentSurface(mTarget);
    }

    void SDLGraphics::endDraw()
    {
        SDL_UpdateTexture(mTargetTexture.get(), nullptr, mTarget->pixels, mTarget->w * sizeof(Uint32));
        SDL_RenderCopy(mRenderer.get(), mTargetTexture.get(), nullptr, nullptr);

        popClipArea();
    }

#define DEPTH 32

    std::shared_ptr<SDL_Surface> createTransparentSurface(size_t width, size_t height)
    {
        std::shared_ptr<SDL_Surface> s;

        // SDL y u do dis
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
        s = std::shared_ptr<SDL_Surface>(SDL_CreateRGBSurface(0, width, height, DEPTH, 0xFF000000, 0x00FF0000, 0x0000FF00, 0x000000FF));
#else
        s = std::shared_ptr<SDL_Surface>(SDL_CreateRGBSurface(0, width, height, DEPTH, 0x000000FF, 0x0000FF00, 0x00FF0000, 0xFF000000),
                                         DeleteSdlSurface);
#endif

        clearTransparentSurface(s);

        return s;
    }

    void SDLGraphics::setTarget(std::shared_ptr<SDL_Renderer> renderer, size_t width, size_t height)
    {
        if(mTarget)
        {
            mTarget=nullptr;
            mTargetTexture=nullptr;
        }

        mTarget = createTransparentSurface(width, height);
        mTargetTexture = std::shared_ptr<SDL_Texture>(SDL_CreateTexture(renderer.get(),
                SDL_PIXELFORMAT_ARGB8888, SDL_TEXTUREACCESS_STREAMING, width, height), DeleteSdlTexture);
        mRenderer = renderer;
    }

    bool SDLGraphics::pushClipArea(gui::Rectangle area)
    {
        SDL_Rect rect;
        bool result = Graphics::pushClipArea(area);

        const gui::ClipRectangle& carea = mClipStack.top();
        rect.x = carea.x;
        rect.y = carea.y;
        rect.w = carea.width;
        rect.h = carea.height;

        SDL_SetClipRect(mTarget.get(), &rect);

        return result;
    }

    void SDLGraphics::popClipArea()
    {
        Graphics::popClipArea();

        if (mClipStack.empty())
        {
            return;
        }

        const gui::ClipRectangle& carea = mClipStack.top();
        SDL_Rect rect;
        rect.x = carea.x;
        rect.y = carea.y;
        rect.w = carea.width;
        rect.h = carea.height;

        SDL_SetClipRect(mTarget.get(), &rect);
    }

    std::shared_ptr<SDL_Surface> SDLGraphics::getTarget() const
    {
        return mTarget;
    }

    void SDLGraphics::drawImage(std::shared_ptr<const gui::ZImage> image,
                                int srcX,
                                int srcY,
                                int dstX,
                                int dstY,
                                int width,
                                int height)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        SDL_Rect src;
        SDL_Rect dst;
        src.x = srcX;
        src.y = srcY;
        src.w = width;
        src.h = height;
        dst.x = dstX + top.xOffset;
        dst.y = dstY + top.yOffset;

        auto srcImage = std::dynamic_pointer_cast<const SDLImage>(image);

        if (srcImage == nullptr)
        {
            throw GCN_EXCEPTION("Trying to draw an image of unknown format, must be an SDLImage.");
        }

        SDL_BlitSurface(srcImage->getSurface().get(), &src, mTarget.get(), &dst);
    }

    void SDLGraphics::fillRectangle(const gui::Rectangle& rectangle)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        gui::Rectangle area = rectangle;
        area.x += top.xOffset;
        area.y += top.yOffset;

        if(!area.isIntersecting(top))
        {
            return;
        }

        if (mAlpha)
        {
            int x1 = area.x > top.x ? area.x : top.x;
            int y1 = area.y > top.y ? area.y : top.y;
            int x2 = area.x + area.width < top.x + top.width ? area.x + area.width : top.x + top.width;
            int y2 = area.y + area.height < top.y + top.height ? area.y + area.height : top.y + top.height;
            int x, y;

            SDL_LockSurface(mTarget.get());
            for (y = y1; y < y2; y++)
            {
                for (x = x1; x < x2; x++)
                {
                    SDLputPixelAlpha(mTarget, x, y, mColor);
                }
            }
            SDL_UnlockSurface(mTarget.get());

        }
        else
        {
            SDL_Rect rect;
            rect.x = area.x;
            rect.y = area.y;
            rect.w = area.width;
            rect.h = area.height;

            Uint32 color = SDL_MapRGBA(mTarget->format, mColor.r, mColor.g, mColor.b, mColor.a);
            SDL_FillRect(mTarget.get(), &rect, color);
        }
    }

    void SDLGraphics::drawPoint(int x, int y)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y += top.yOffset;

        if(!top.isContaining(x,y))
            return;

        if (mAlpha)
        {
            SDLputPixelAlpha(mTarget, x, y, mColor);
        }
        else
        {
            SDLputPixel(mTarget, x, y, mColor);
        }
    }

    void SDLGraphics::drawHLine(int x1, int y, int x2)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        y += top.yOffset;
        x2 += top.xOffset;

        if (y < top.y || y >= top.y + top.height)
        {
            return;
        }

        if (x1 > x2)
        {
            x1 ^= x2;
            x2 ^= x1;
            x1 ^= x2;
        }

        if (top.x > x1)
        {
            if (top.x > x2)
            {
                return;
            }

            x1 = top.x;
        }

        if (top.x + top.width <= x2)
        {
            if (top.x + top.width <= x1)
            {
                return;
            }

            x2 = top.x + top.width -1;
        }

        int bpp = mTarget->format->BytesPerPixel;

        SDL_LockSurface(mTarget.get());

        Uint8 *p = (Uint8 *)mTarget->pixels + y * mTarget->pitch + x1 * bpp;

        Uint32 pixel = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);

        switch(bpp)
        {
            case 1:
                for (;x1 <= x2; ++x1)
                {
                    *(p++) = pixel;
                }
                break;

            case 2:
            {
                Uint16* q = (Uint16*)p;
                for (;x1 <= x2; ++x1)
                {
                    *(q++) = pixel;
                }
                break;
            }
            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    for (;x1 <= x2; ++x1)
                    {
                        p[0] = (pixel >> 16) & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = pixel & 0xff;
                        p += 3;
                    }
                }
                else
                {
                    for (;x1 <= x2; ++x1)
                    {
                        p[0] = pixel & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = (pixel >> 16) & 0xff;
                        p += 3;
                    }
                }
                break;

            case 4:
            {
                Uint32* q = (Uint32*)p;
                for (;x1 <= x2; ++x1)
                {
                    if (mAlpha)
                    {
                        *q = SDLAlpha32(pixel,*q,mColor.a);
                        q++;
                    }
                    else
                    {
                        *(q++) = pixel;
                    }
                }
                break;
            }

        } // end switch

        SDL_UnlockSurface(mTarget.get());
    }

    void SDLGraphics::drawVLine(int x, int y1, int y2)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        x += top.xOffset;
        y1 += top.yOffset;
        y2 += top.yOffset;

        if (x < top.x || x >= top.x + top.width)
        {
            return;
        }

        if (y1 > y2)
        {
            y1 ^= y2;
            y2 ^= y1;
            y1 ^= y2;
        }

        if (top.y > y1)
        {
            if (top.y > y2)
            {
                return;
            }

            y1 = top.y;
        }

        if (top.y + top.height <= y2)
        {
            if (top.y + top.height <= y1)
            {
                return;
            }

            y2 = top.y + top.height - 1;
        }

        int bpp = mTarget->format->BytesPerPixel;

        SDL_LockSurface(mTarget.get());

        Uint8 *p = (Uint8 *)mTarget->pixels + y1 * mTarget->pitch + x * bpp;

        Uint32 pixel = SDL_MapRGB(mTarget->format, mColor.r, mColor.g, mColor.b);

        switch(bpp)
        {
            case 1:
                for (;y1 <= y2; ++y1)
                {
                    *p = pixel;
                    p += mTarget->pitch;
                }
                break;

            case 2:
                for (;y1 <= y2; ++y1)
                {
                    *(Uint16*)p = pixel;
                    p += mTarget->pitch;
                }
                break;

            case 3:
                if(SDL_BYTEORDER == SDL_BIG_ENDIAN)
                {
                    for (;y1 <= y2; ++y1)
                    {
                        p[0] = (pixel >> 16) & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = pixel & 0xff;
                        p += mTarget->pitch;
                    }
                }
                else
                {
                    for (;y1 <= y2; ++y1)
                    {
                        p[0] = pixel & 0xff;
                        p[1] = (pixel >> 8) & 0xff;
                        p[2] = (pixel >> 16) & 0xff;
                        p += mTarget->pitch;
                    }
                }
                break;

            case 4:
                for (;y1 <= y2; ++y1)
                {
                    if (mAlpha)
                    {
                        *(Uint32*)p = SDLAlpha32(pixel,*(Uint32*)p,mColor.a);
                    }
                    else
                    {
                        *(Uint32*)p = pixel;
                    }
                    p += mTarget->pitch;
                }
                break;

        } // end switch

        SDL_UnlockSurface(mTarget.get());
    }

    void SDLGraphics::drawRectangle(const gui::Rectangle& rectangle)
    {
        int x1 = rectangle.x;
        int x2 = rectangle.x + rectangle.width - 1;
        int y1 = rectangle.y;
        int y2 = rectangle.y + rectangle.height - 1;

        drawHLine(x1, y1, x2);
        drawHLine(x1, y2, x2);

        drawVLine(x1, y1, y2);
        drawVLine(x2, y1, y2);
    }

    void SDLGraphics::drawLine(int x1, int y1, int x2, int y2)
    {
        if (x1 == x2)
        {
            drawVLine(x1, y1, y2);
            return;
        }
        if (y1 == y2)
        {
            drawHLine(x1, y1, x2);
            return;
        }

        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        x1 += top.xOffset;
        y1 += top.yOffset;
        x2 += top.xOffset;
        y2 += top.yOffset;

        // Draw a line with Bresenham

        int dx = std::abs(x2 - x1);
        int dy = std::abs(y2 - y1);

        if (dx > dy)
        {
            if (x1 > x2)
            {
                // swap x1, x2
                x1 ^= x2;
                x2 ^= x1;
                x1 ^= x2;

                // swap y1, y2
                y1 ^= y2;
                y2 ^= y1;
                y1 ^= y2;
            }

            if (y1 < y2)
            {
                int y = y1;
                int p = 0;

                for (int x = x1; x <= x2; x++)
                {
                    if (top.isContaining(x, y))
                    {
                        if (mAlpha)
                        {
                            SDLputPixelAlpha(mTarget, x, y, mColor);
                        }
                        else
                        {
                            SDLputPixel(mTarget, x, y, mColor);
                        }
                    }

                    p += dy;

                    if (p * 2 >= dx)
                    {
                        y++;
                        p -= dx;
                    }
                }
            }
            else
            {
                int y = y1;
                int p = 0;

                for (int x = x1; x <= x2; x++)
                {
                    if (top.isContaining(x, y))
                    {
                        if (mAlpha)
                        {
                            SDLputPixelAlpha(mTarget, x, y, mColor);
                        }
                        else
                        {
                            SDLputPixel(mTarget, x, y, mColor);
                        }
                    }

                    p += dy;

                    if (p * 2 >= dx)
                    {
                        y--;
                        p -= dx;
                    }
                }
            }
        }
        else
        {
            if (y1 > y2)
            {
                // swap y1, y2
                y1 ^= y2;
                y2 ^= y1;
                y1 ^= y2;

                // swap x1, x2
                x1 ^= x2;
                x2 ^= x1;
                x1 ^= x2;
            }

            if (x1 < x2)
            {
                int x = x1;
                int p = 0;

                for (int y = y1; y <= y2; y++)
                {
                    if (top.isContaining(x, y))
                    {
                        if (mAlpha)
                        {
                            SDLputPixelAlpha(mTarget, x, y, mColor);
                        }
                        else
                        {
                            SDLputPixel(mTarget, x, y, mColor);
                        }
                    }

                    p += dx;

                    if (p * 2 >= dy)
                    {
                        x++;
                        p -= dy;
                    }
                }
            }
            else
            {
                int x = x1;
                int p = 0;

                for (int y = y1; y <= y2; y++)
                {
                    if (top.isContaining(x, y))
                    {
                        if (mAlpha)
                        {
                            SDLputPixelAlpha(mTarget, x, y, mColor);
                        }
                        else
                        {
                            SDLputPixel(mTarget, x, y, mColor);
                        }
                    }

                    p += dx;

                    if (p * 2 >= dy)
                    {
                        x--;
                        p -= dy;
                    }
                }
            }
        }
    }

    void SDLGraphics::setColor(const gui::ZColor& color)
    {
        mColor = color;

        mAlpha = color.a != 255;
    }

    const gui::ZColor& SDLGraphics::getColor() const
    {
        return mColor;
    }

    void SDLGraphics::drawSDLSurface(std::shared_ptr<SDL_Surface> surface, SDL_Rect source, SDL_Rect destination)
    {
        if (mClipStack.empty())
        {
            throw GCN_EXCEPTION("Clip stack is empty, perhaps you called a draw funtion outside of _beginDraw() and _endDraw()?");
        }

        const gui::ClipRectangle& top = mClipStack.top();

        destination.x += top.xOffset;
        destination.y += top.yOffset;

        SDL_BlitSurface(surface.get(), &source, mTarget.get(), &destination);
    }
}