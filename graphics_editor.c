#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define WIDTH 50
#define HEIGHT 20
#define MAX_OBJECTS 100

char canvas[HEIGHT][WIDTH];

typedef struct
{
    char type;
    int x, y;
    int w, h;
} Object;

Object objects[MAX_OBJECTS];
int objectCount = 0;

void initCanvas()
{
    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            canvas[i][j] = '_';
        }
    }
}

void displayCanvas()
{
    printf("\n");

    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            printf("%c ", canvas[i][j]);
        }
        printf("\n");
    }
}

void drawRectangle(int x, int y, int w, int h)
{
    for(int i = x; i < x + w; i++)
    {
        if(i >= 0 && i < WIDTH)
        {
            if(y >= 0 && y < HEIGHT)
                canvas[y][i] = '*';

            if(y + h - 1 >= 0 && y + h - 1 < HEIGHT)
                canvas[y + h - 1][i] = '*';
        }
    }

    for(int j = y; j < y + h; j++)
    {
        if(j >= 0 && j < HEIGHT)
        {
            if(x >= 0 && x < WIDTH)
                canvas[j][x] = '*';

            if(x + w - 1 >= 0 && x + w - 1 < WIDTH)
                canvas[j][x + w - 1] = '*';
        }
    }
}

void drawLine(int x1, int y1, int x2, int y2)
{
    int dx = x2 - x1;
    int dy = y2 - y1;

    int steps = abs(dx) > abs(dy) ? abs(dx) : abs(dy);

    float xInc = (float)dx / steps;
    float yInc = (float)dy / steps;

    float x = x1;
    float y = y1;

    for(int i = 0; i <= steps; i++)
    {
        if((int)x >= 0 && (int)x < WIDTH &&
           (int)y >= 0 && (int)y < HEIGHT)
        {
            canvas[(int)y][(int)x] = '*';
        }

        x += xInc;
        y += yInc;
    }
}

void drawCircle(int cx, int cy, int r)
{
    int x = 0;
    int y = r;
    int p = 1 - r;

    while(x <= y)
    {
        int pts[8][2] =
        {
            {cx+x, cy+y},
            {cx-x, cy+y},
            {cx+x, cy-y},
            {cx-x, cy-y},
            {cx+y, cy+x},
            {cx-y, cy+x},
            {cx+y, cy-x},
            {cx-y, cy-x}
        };

        for(int i = 0; i < 8; i++)
        {
            int px = pts[i][0];
            int py = pts[i][1];

            if(px >= 0 && px < WIDTH &&
               py >= 0 && py < HEIGHT)
            {
                canvas[py][px] = '*';
            }
        }

        x++;

        if(p < 0)
            p += 2 * x + 1;
        else
        {
            y--;
            p += 2 * (x - y) + 1;
        }
    }
}

void drawTriangle(int x, int y, int size)
{
    drawLine(x, y, x + size, y);
    drawLine(x, y, x + size / 2, y - size);
    drawLine(x + size, y, x + size / 2, y - size);
}

void redrawObjects()
{
    initCanvas();

    for(int i = 0; i < objectCount; i++)
    {
        Object o = objects[i];

        switch(o.type)
        {
            case 'R':
                drawRectangle(o.x, o.y, o.w, o.h);
                break;

            case 'L':
                drawLine(o.x, o.y, o.w, o.h);
                break;

            case 'C':
                drawCircle(o.x, o.y, o.w);
                break;

            case 'T':
                drawTriangle(o.x, o.y, o.w);
                break;
        }
    }
}

void addObject(char type, int x, int y, int w, int h)
{
    if(objectCount >= MAX_OBJECTS)
    {
        printf("Object limit reached!\n");
        return;
    }

    objects[objectCount].type = type;
    objects[objectCount].x = x;
    objects[objectCount].y = y;
    objects[objectCount].w = w;
    objects[objectCount].h = h;

    objectCount++;

    redrawObjects();
}

void deleteObject(int index)
{
    if(index < 0 || index >= objectCount)
    {
        printf("Invalid ID!\n");
        return;
    }

    for(int i = index; i < objectCount - 1; i++)
    {
        objects[i] = objects[i + 1];
    }

    objectCount--;

    redrawObjects();
}

void modifyObject(int index, int x, int y, int w, int h)
{
    if(index < 0 || index >= objectCount)
    {
        printf("Invalid ID!\n");
        return;
    }

    objects[index].x = x;
    objects[index].y = y;
    objects[index].w = w;
    objects[index].h = h;

    redrawObjects();
}

void listObjects()
{
    printf("\nObjects in Scene:\n");

    for(int i = 0; i < objectCount; i++)
    {
        printf("ID %d -> Type %c\n",
               i,
               objects[i].type);
    }
}

void saveCanvas()
{
    FILE *fp = fopen("drawing.txt", "w");

    if(fp == NULL)
    {
        printf("File Error!\n");
        return;
    }

    for(int i = 0; i < HEIGHT; i++)
    {
        for(int j = 0; j < WIDTH; j++)
        {
            fputc(canvas[i][j], fp);
        }
        fputc('\n', fp);
    }

    fclose(fp);

    printf("Drawing saved to drawing.txt\n");
}

int main()
{
    int choice;

    initCanvas();

    while(1)
    {
        printf("\n===== 2D GRAPHICS EDITOR =====\n");
        printf("1. Add Rectangle\n");
        printf("2. Add Line\n");
        printf("3. Add Circle\n");
        printf("4. Add Triangle\n");
        printf("5. Delete Object\n");
        printf("6. Modify Object\n");
        printf("7. List Objects\n");
        printf("8. Display Canvas\n");
        printf("9. Save Drawing\n");
        printf("0. Exit\n");

        printf("Enter choice: ");
        scanf("%d", &choice);

        if(choice == 0)
            break;

        if(choice >= 1 && choice <= 4)
        {
            int x, y, w, h;

            if(choice == 1)
            {
                printf("x y width height: ");
                scanf("%d%d%d%d", &x, &y, &w, &h);

                addObject('R', x, y, w, h);
            }

            else if(choice == 2)
            {
                int x2, y2;

                printf("x1 y1 x2 y2: ");
                scanf("%d%d%d%d", &x, &y, &x2, &y2);

                addObject('L', x, y, x2, y2);
            }

            else if(choice == 3)
            {
                int r;

                printf("centerX centerY radius: ");
                scanf("%d%d%d", &x, &y, &r);

                addObject('C', x, y, r, 0);
            }

            else if(choice == 4)
            {
                int size;

                printf("x y size: ");
                scanf("%d%d%d", &x, &y, &size);

                addObject('T', x, y, size, 0);
            }
        }

        else if(choice == 5)
        {
            int id;

            listObjects();

            printf("Enter ID to delete: ");
            scanf("%d", &id);

            deleteObject(id);
        }

        else if(choice == 6)
        {
            int id, x, y, w, h;

            listObjects();

            printf("Enter ID to modify: ");
            scanf("%d", &id);

            printf("New x y w h: ");
            scanf("%d%d%d%d", &x, &y, &w, &h);

            modifyObject(id, x, y, w, h);
        }

        else if(choice == 7)
        {
            listObjects();
        }

        else if(choice == 8)
        {
            displayCanvas();
        }

        else if(choice == 9)
        {
            saveCanvas();
        }
    }

    printf("Exiting Graphics Editor...\n");

    return 0;
}
