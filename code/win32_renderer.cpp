#include "geometry.h"

#define NOMINMAX
#include <windows.h>
#include <stdint.h>
#include <stdlib.h>

#include <math.h>


//For convenience we define three keywords for the three uses that the keyword "static"
//Is going to have in our project
#define internal_func static
#define local_persist static;
#define global_variable static;

struct win32_offscreen_buffer
{
    //Pixels are always 32-bits wide, 
    //Little Endian 0x XX RR GG BB, Memory order BB GG RR XX
    BITMAPINFO Info;
    void* Memory;
    int Width;
    int Height;
    int Pitch;
};

struct win32_window_dimension
{
    int Width;
    int Height;
};

struct render_camera
{
    Vec3f Pos;
    Matrix44f CameraToWorld;
    float CanvasDistance;
    float CanvasWidth;
    float CanvasHeight;
}

//Made global temporarily(Probably)
global_variable bool GlobalRunning;   //static variable initialized to 0 by default
global_variable win32_offscreen_buffer GlobalBackbuffer;


internal_func void
RotateHue(int *Red, int *Green, int *Blue)
{
    if(*Red == 255)
    {
        if(*Green > 0)
        {
            --*Green;
        }
        else if(*Blue < 255)
        {
            ++*Blue;
        }
    }
    if(*Green == 255)
    {
        if(*Blue > 0)
        {
            --*Blue;
        }
        else if(*Red < 255)
        {
            ++*Red;
        }
    }
    if(*Blue == 255)
    {
        if(*Red > 0)
        {
            --*Red;
        }
        else if(*Green < 255)
        {
            ++*Green;
        }
    }
}

internal_func void
CleanBuffer(win32_offscreen_buffer Buffer)
{
    uint8_t *Row = (uint8_t *)Buffer.Memory; 
    for(int Y = 0; Y < Buffer.Height; Y++)
    {
        uint32_t *Pixel = (uint32_t *)Row;
        for(int X = 0; X < Buffer.Width; X++)
        {
            *Pixel = 0;
            Pixel++;
        }
        Row += GlobalBackbuffer.Pitch;
    }
}

internal_func void
RenderRect(win32_offscreen_buffer Buffer, int Top, int Left, int Width, int Height, uint32_t RGB)
{
    
    uint8_t *Row = (uint8_t *)Buffer.Memory;
    Row += Buffer.Pitch * Top;
    for(int Y = 0; Y < Height; Y++)
    {
        uint32_t *Pixel = (uint32_t *)Row;
        Pixel += Left; 
        for(int X = 0; X < Width; X++)
        {
            *Pixel = RGB;
            Pixel++;
        } 
        Row += GlobalBackbuffer.Pitch;
    }

}

void DrawLineSegment(win32_offscreen_buffer Buffer, int Size, int X, int Y)
{
    if(X < Buffer.Width && X >= 0 && Y < Buffer.Height && Y >= 0)
    {
        uint8_t *Row = (uint8_t *)Buffer.Memory;
        Row += Buffer.Pitch * Y;
        for(int i = 0; i < Size; i++)
        {
            uint32_t *Pixel = (uint32_t *)Row;
            Pixel += X;
            for(int j = 0; j < Size; j++)
            {
                *Pixel = 255;
                Pixel++;
            }
            Row += Buffer.Pitch;
        }   
    }
 
}

internal_func void
RenderGradient(win32_offscreen_buffer Buffer, int XOffset, int YOffset)
{
    uint8_t *Row = (uint8_t *)Buffer.Memory; 
    for(int Y = 0; Y < Buffer.Height; Y++)
    {
        uint32_t *Pixel = (uint32_t *)Row;
        for(int X = 0; X < Buffer.Width; X++)
        {
            
            uint8_t Blue = (X + XOffset);
            uint8_t Green = (Y + YOffset);

            *Pixel = ( (Green << 8)|Blue );
            Pixel++;
        }
        Row += GlobalBackbuffer.Pitch;
    }
}

internal_func void
RenderLine(win32_offscreen_buffer Buffer, Vec2i p0, Vec2i p1)
{
    int LineWidth = 1;
    float m;
    
    p0.x = std::max(0, p0.x); p0.x = std::min(p0.x, Buffer.Width-1);
    p0.y = std::max(0, p0.y); p0.y = std::min(p0.y, Buffer.Height-1);

    p1.x = std::max(0, p1.x); p1.x = std::min(p1.x, Buffer.Width-1);
    p1.y = std::max(0, p1.y); p1.y = std::min(p1.y, Buffer.Height-1);

    //If line is no completly vertical
    if(p0.x != p1.x)
    {
        //Get slope
        m = (float)(p1.y - p0.y)/(float)(p1.x - p0.x);
    }

    //If line not vertical and slope is mainly horizontal
    if(p0.x != p1.x && std::abs(m) <= 1)
    {
        //Rearrange so point with lower X is p0
        if(p1.x < p0.x)
        {
            std::swap(p0, p1);
        }

        //Get vertical offset of line
        float b = p0.y - m * p0.x;

        for(int xx = p0.x; xx <= p1.x; xx++)
        {
            int yy = m * xx + b;
            DrawLineSegment(Buffer, LineWidth, xx, yy);
        }
    }
    //If slope is mainly vertical
    else
    {
        //If Y is equal it means they're the same point
        if(p0.y != p1.y)
        {
            //Rearrange so point with lower Y is p0
            if(p1.y < p0.y)
            {
                std::swap(p0, p1);
            }

            //Get inverted slope
            float w = (float)(p1.x - p0.x)/(float)(p1.y - p0.y);
            //Get horizontal offset of line
            float p = p0.x - w * p0.y;

            for(int yy = p0.y; yy <= p1.y; yy++)
            {
                int xx = w * yy + p;
                DrawLineSegment(Buffer, LineWidth, xx, yy);
            }
        }
        else
        {
            DrawLineSegment(Buffer, LineWidth, p0.x, p0.y);
        }
        
    }

}

void RenderTriangle(win32_offscreen_buffer Buffer, render_camera Camera, Vec3f v0, Vec3f v1, Vec3f v2, int ImageWidth, int ImageHeight)
{
    Vec3f PolNormal = v0.CrossProduct(v1);
}


Vec2i ProjectPoint(Matrix44f &WorldToCamera, Vec3f &V, render_camera Camera, int ImageWidth, int ImageHeight)
{
    Vec3f PCamera;
    WorldToCamera.MultPointMatrix(V, PCamera);

    Vec3f PScreen;

    PScreen.x = (PCamera.x * Camera.CanvasDistance)/(- PCamera.z);
    PScreen.y = (PCamera.y * Camera.CanvasDistance)/(- PCamera.z);
    PScreen.z = Camera.CanvasDistance;

    Vec2f PNDC;
    PNDC.x = (PScreen.x + Camera.CanvasWidth /2) / (Camera.CanvasWidth);
    PNDC.y = (PScreen.y + Camera.CanvasHeight /2) / (Camera.CanvasHeight);

    Vec2i PRaster;
    PRaster.x = (int)(PNDC.x * (float)ImageWidth);
    PRaster.y = (int)(PNDC.y * (float)ImageHeight);

    return PRaster;
}


win32_window_dimension
Win32GetWindowDimension(HWND Window)
{
    win32_window_dimension Result;

    RECT ClientRect;
    GetClientRect(Window, &ClientRect);
    Result.Height = ClientRect.bottom - ClientRect.top; 
    Result.Width = ClientRect.right - ClientRect.left;

    return Result;
}

//Resize a Device Independent Bitmap: term for user bitmap used for rendering 
internal_func void
Win32ResizeDIBSection(win32_offscreen_buffer *Buffer, int Width, int Height)
{
    if(Buffer->Memory)
    {
        VirtualFree(Buffer->Memory, 0, MEM_RELEASE);
    }

    Buffer->Width = Width;
    Buffer->Height = Height;

    Buffer->Info.bmiHeader.biSize = sizeof(Buffer->Info.bmiHeader);
    Buffer->Info.bmiHeader.biWidth = Buffer->Width;
    //Negative height is interpreted as origin being in top-left
    Buffer->Info.bmiHeader.biHeight = -Buffer->Height;
    Buffer->Info.bmiHeader.biPlanes = 1;
    Buffer->Info.bmiHeader.biBitCount = 32;
    Buffer->Info.bmiHeader.biCompression = BI_RGB;

    //Altough RGB only needs 3 bytes to store, in order to keep aligned
    //With 32 bit words we add a byte for padding 
    int BytesPerPixel = 4;      
    int BitmapMemorySize = (Buffer->Width*Buffer->Height) * BytesPerPixel;

    GlobalBackbuffer.Pitch = GlobalBackbuffer.Width*BytesPerPixel;

    //Using VirtualAlloc instead of HeapAlloc to allocate memory
    //To have our own page and have more freedom to 
    //Do memory allocation ourselves
    Buffer->Memory = VirtualAlloc(0, BitmapMemorySize, MEM_COMMIT, PAGE_READWRITE);

    //TODO: Probably clear bitmap to black
}

internal_func void
Win32DisplayBufferInWindow(HDC DeviceContext, 
                            int WindowWidth, int WindowHeight,
                            win32_offscreen_buffer Buffer)
{    
    //TODO: aspect ratio correction
    int res = StretchDIBits(DeviceContext,
                    0, 0, WindowWidth, WindowHeight,        //Destination Rectangle
                    0, 0, Buffer.Width, Buffer.Height,      //Source Rectangle
                    Buffer.Memory,
                    &Buffer.Info,
                    DIB_RGB_COLORS, SRCCOPY);
    
}

//User defined callback to respond to messages from the queue
LRESULT
Win32MainWindowCallback(HWND Window,
                    UINT Message,
                    WPARAM WParam,
                    LPARAM LParam
)
{
    LRESULT Result = 0;

    switch(Message)
    {
        //We define behaivour for messages we specifically want control over
        case WM_DESTROY:
        {
            //TODO: Hanndle as error?
            GlobalRunning = false;
        }break;

        case WM_CLOSE:
        {
            GlobalRunning = false;
        }break;

        case WM_ACTIVATE:
        {
            OutputDebugString("WM_ACTIVATE\n");
        }break;

        //Windows requires behaivour for WM_PAINT on certain occasions
        //Necessary to have BeginPaint and EndPaint if not handled with default proc
        case WM_PAINT:
        {
            PAINTSTRUCT Paint;
            HDC DeviceContext = BeginPaint(Window, &Paint); 
            
            int X = Paint.rcPaint.left;
            int Y = Paint.rcPaint.top;
            int Height = Paint.rcPaint.bottom - Paint.rcPaint.top; 
            int Width = Paint.rcPaint.right - Paint.rcPaint.left;
            
            
            win32_window_dimension Dimension = Win32GetWindowDimension(Window);

            Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height, 
                                        GlobalBackbuffer);

            EndPaint(Window, &Paint);
        }break;

        default:
        {
            //Use default proc to handle message
            Result = DefWindowProc(Window, Message, WParam, LParam);
        }break;
    }

    return Result;
}

//Entry point for the C compiler
INT CALLBACK 
WinMain(HINSTANCE hInstance, 
    HINSTANCE hPrevInstance,
    PSTR lpCmdLine, 
    INT nCmdShow)
{
    //Declare Windows class struct and initialize to 0
    WNDCLASS WindowClass = {0};

    Win32ResizeDIBSection(&GlobalBackbuffer, 1200, 720);

    WindowClass.style = CS_HREDRAW|CS_VREDRAW;
    WindowClass.lpfnWndProc = Win32MainWindowCallback;
    WindowClass.hInstance = hInstance;
    WindowClass.lpszClassName = "RendererWindowClass";

    //Register window class to create a window later
    if(RegisterClass(&WindowClass))
    {
        //Create window and sotre window handle
        HWND Window =
             CreateWindowEx(0,
                            WindowClass.lpszClassName,
                            "Renderer",
                            WS_OVERLAPPEDWINDOW|WS_VISIBLE,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            CW_USEDEFAULT,
                            0,
                            0,
                            hInstance,
                            0);

        if(Window != NULL)
        {
            int XOffset = 0;
            int YOffset = 0;

            int Red = 255;
            int Green = 0;
            int Blue = 0;
            //Main window loop
            GlobalRunning = true;
            
            /*
            Vec3f Verts[6] = {
                {0, 0, 0}, {1, 1, 0}, {0, 1, 1}, {-1, 1, 0}, {0, 1, -1}, {0, 2, 0}
            };

            const int NumTris = 8;

            uint32_t Tris[NumTris * 3] = 
            {0, 1, 2, 
            0, 1, 4,
            0, 3, 4,
            0, 2, 3,
            5, 1, 2,
            5, 1, 4,
            5, 3, 4,
            5, 3, 2};
            */

            Vec3f Verts[8] = {
                {1, 1, 1}, {1, -1, 1}, {-1, -1, 1}, {-1, 1, 1},
                {1, 1, -1}, {1, -1, -1}, {-1, -1, -1}, {-1, 1, -1}
            };

            const int NumTris = 12;

            uint32_t Tris[NumTris * 3] = 
            {0, 1, 2,   2, 3, 0,
            0, 4, 1,    4, 5, 1,
            5, 1, 2,    2, 6, 5,
            3, 2, 6,    6, 7, 3,
            4, 0, 3,    3, 7, 4,
            4, 5, 6,    6, 7, 4};

            Vec3f ObjPos(0, 0, 10);
            float ObjScale = 1;
            Matrix44f TranslMatrix(ObjScale, 0, 0, 0,
                                  0, ObjScale, 0, 0,
                                  0, 0, ObjScale, 0, 
                                  ObjPos.x, ObjPos.y, ObjPos.z, 1);
            float RotAngle = 0.001f;
            Matrix44f RotYMatrix(cosf(RotAngle),     0,  -sinf(RotAngle),     0,
                                0,                  1,                0,     0,
                                sinf(RotAngle),     0,   cosf(RotAngle),     0,
                                0,                  0,                0,     1);
            Matrix44f RotZMatrix(cosf(RotAngle*2),    sinf(RotAngle*2),   0,     0,
                                -sinf(RotAngle*2),    cosf(RotAngle*2),   0,     0,
                                0,                  0,                1,     0,
                                0,                  0,                0,     1);      

            Matrix44f RotMatrix = RotYMatrix; 
            /*
            for(int i = 0; i < 6; i++)
            {
                Vec3f temp;
                TransMatrix.MultPointMatrix(Verts[i], temp);
                Verts[i] = temp;
            }
            */

            render_camera Camera;
            Camera.CameraToWorld = Matrix44f(1, 0, 0, 0, 
                                            0, 1, 0, 0, 
                                            0, 0, -1, 0, 
                                            0, 0, 1, 1);
            Camera.CanvasDistance = 5;
            Camera.CanvasWidth = 16;
            Camera.CanvasHeight = 9;

            Matrix44f WorldToCamera = Camera.CameraToWorld.Inverse();

            while(GlobalRunning)
            {
                

                //Receive all messages from message queue
                MSG Message;
                while(PeekMessage(&Message, 0, 0, 0, PM_REMOVE)) //Return of 0 means failure to find message
                {
                    if(Message.message == WM_QUIT)
                    {
                        GlobalRunning = false;
                    }

                    TranslateMessage(&Message);
                    DispatchMessage(&Message);
                }

                //RenderGradient(GlobalBackbuffer, XOffset, YOffset);
                
                CleanBuffer(GlobalBackbuffer);
                /*
                uint32_t RGB = (uint32_t)((Red << 16) | (Green << 8)) | Blue;

                //Test animation
                int VWaveOffset = (int)( sinf((float)XOffset*2 / 100) * 150 );    
                int HWaveOffset = (int)( cosf((float)XOffset / 100) * 150 );
                RenderRect(GlobalBackbuffer, 300 + VWaveOffset, 500 + HWaveOffset, 100, 100, RGB);
                */
                Vec3f ModVerts[8];

                for(int i = 0; i < 8; i++)
                {
                    Vec3f temp;
                    RotMatrix.MultPointMatrix(Verts[i], temp);
                    Verts[i] = temp;
                    TranslMatrix.MultPointMatrix(Verts[i], ModVerts[i]);   
                }


                for(int i = 0; i < NumTris; i++)
                {
                    
                    Vec3f v0 = ModVerts[Tris[i * 3]];;
                    Vec3f v1 = ModVerts[Tris[i * 3 + 1]];;
                    Vec3f v2 = ModVerts[Tris[i * 3 + 2]];;


                    int Width = GlobalBackbuffer.Width;
                    int Height = GlobalBackbuffer.Height;
                    Vec2i p0 = ProjectPoint(WorldToCamera, v0, Camera, Width, Height);
                    Vec2i p1 = ProjectPoint(WorldToCamera, v1, Camera, Width, Height);
                    Vec2i p2 = ProjectPoint(WorldToCamera, v2, Camera, Width, Height);

                    RenderLine(GlobalBackbuffer, p0, p1);
                    RenderLine(GlobalBackbuffer, p1, p2);
                    RenderLine(GlobalBackbuffer, p2, p0);
                }

                HDC DeviceContext = GetDC(Window);

                win32_window_dimension Dimension = Win32GetWindowDimension(Window);

                Win32DisplayBufferInWindow(DeviceContext, Dimension.Width, Dimension.Height, 
                                            GlobalBackbuffer);
                ReleaseDC(Window, DeviceContext);
                
            }
        }
        else
        {
            //TODO: Logging
        }
    }
    else
    {
        //TODO: Logging
    }

    return(0);
}