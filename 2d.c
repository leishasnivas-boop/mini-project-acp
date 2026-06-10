#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define W 80
#define H 24

typedef enum { LINE = 1, RECT, CIRCLE, TRI } Type;

typedef struct {
    int id;
    int active;
    Type type;
    int d[6];
} Shape;

char canvas[H][W];
Shape shapes[100];
int count = 0;

void clear_canvas() {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) { canvas[y][x] = '_'; }
    }
}

void plot(int x, int y) {
    if (x >= 0 && x < W && y >= 0 && y < H) { canvas[y][x] = '*'; }
}

void draw_line(int x0, int y0, int x1, int y1) {
    int dx = abs(x1 - x0), sx = x0 < x1 ? 1 : -1;
    int dy = -abs(y1 - y0), sy = y0 < y1 ? 1 : -1;
    int err = dx + dy, e2;
    while (1) {
        plot(x0, y0);
        if (x0 == x1 && y0 == y1) break;
        e2 = 2 * err;
        if (e2 >= dy) { err += dy; x0 += sx; }
        if (e2 <= dx) { err += dx; y0 += sy; }
    }
}

void draw_circle(int xc, int yc, int r) {
    int x = 0, y = r;
    int d = 3 - 2 * r;
    while (y >= x) {
        plot(xc + x, yc + y); plot(xc - x, yc + y);
        plot(xc + x, yc - y); plot(xc - x, yc - y);
        plot(xc + y, yc + x); plot(xc - y, yc + x);
        plot(xc + y, yc - x); plot(xc - y, yc - x);
        x++;
        if (d > 0) { y--; d = d + 4 * (x - y) + 10; }
        else { d = d + 4 * x + 6; }
    }
}

void display() {
    clear_canvas();
    for (int i = 0; i < count; i++) {
        if (!shapes[i].active) continue;
        Shape s = shapes[i];
        if (s.type == LINE) { draw_line(s.d[0], s.d[1], s.d[2], s.d[3]); } 
        else if (s.type == RECT) {
            int x1 = s.d[0], y1 = s.d[1], x2 = s.d[2], y2 = s.d[3];
            draw_line(x1, y1, x2, y1); draw_line(x1, y2, x2, y2);
            draw_line(x1, y1, x1, y2); draw_line(x2, y1, x2, y2);
        } else if (s.type == TRI) {
            draw_line(s.d[0], s.d[1], s.d[2], s.d[3]);
            draw_line(s.d[2], s.d[3], s.d[4], s.d[5]);
            draw_line(s.d[4], s.d[5], s.d[0], s.d[1]);
        } else if (s.type == CIRCLE) { draw_circle(s.d[0], s.d[1], s.d[2]); }
    }
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) { putchar(canvas[y][x]); }
        printf("\n");
    }
}

void print_menu() {
    printf("\n2D Graphics Editor\nCanvas size: %d x %d\n1. Add object\n2. Delete object\n3. Modify object\n4. Display picture\n5. List objects\n0. Exit\nEnter choice: ", W, H);
}

int main() {
    int choice, type, id;
    while (1) {
        print_menu();
        if (scanf("%d", &choice) != 1) break;
        if (choice == 0) { printf("Goodbye.\n"); break; }
        else if (choice == 1) {
            printf("Choose shape type:\n1. Line\n2. Rectangle\n3. Circle\n4. Triangle\nEnter shape type: ");
            scanf("%d", &type);
            shapes[count].id = count;
            shapes[count].type = (Type)type;
            shapes[count].active = 1;
            if (type == 1) { printf("Enter x1 y1 x2 y2: "); scanf("%d %d %d %d", &shapes[count].d[0], &shapes[count].d[1], &shapes[count].d[2], &shapes[count].d[3]); }
            else if (type == 2) { printf("Enter top-left x y and bottom-right x y: "); scanf("%d %d %d %d", &shapes[count].d[0], &shapes[count].d[1], &shapes[count].d[2], &shapes[count].d[3]); }
            else if (type == 3) { printf("Enter center x y and radius: "); scanf("%d %d %d", &shapes[count].d[0], &shapes[count].d[1], &shapes[count].d[2]); }
            else if (type == 4) { printf("Enter x1 y1 x2 y2 x3 y3: "); scanf("%d %d %d %d %d %d", &shapes[count].d[0], &shapes[count].d[1], &shapes[count].d[2], &shapes[count].d[3], &shapes[count].d[4], &shapes[count].d[5]); }
            printf("Object added with index %d.\n", count);
        } else if (choice == 2) {
            printf("Enter object index to delete: ");
            scanf("%d", &id);
            if (id >= 0 && id < count) shapes[id].active = 0;
        } else if (choice == 3) {
            printf("Enter object index to modify: ");
            scanf("%d", &id);
            if (id >= 0 && id < count && shapes[id].active) {
                if (shapes[id].type == LINE) { printf("Enter x1 y1 x2 y2: "); scanf("%d %d %d %d", &shapes[id].d[0], &shapes[id].d[1], &shapes[id].d[2], &shapes[id].d[3]); }
                else if (shapes[id].type == RECT) { printf("Enter top-left x y and bottom-right x y: "); scanf("%d %d %d %d", &shapes[id].d[0], &shapes[id].d[1], &shapes[id].d[2], &shapes[id].d[3]); }
                else if (shapes[id].type == CIRCLE) { printf("Enter center x y and radius: "); scanf("%d %d %d", &shapes[id].d[0], &shapes[id].d[1], &shapes[id].d[2]); }
                else if (shapes[id].type == TRI) { printf("Enter x1 y1 x2 y2 x3 y3: "); scanf("%d %d %d %d %d %d", &shapes[id].d[0], &shapes[id].d[1], &shapes[id].d[2], &shapes[id].d[3], &shapes[id].d[4], &shapes[id].d[5]); }
            }
        } else if (choice == 4) { display(); }
        else if (choice == 5) {
            for (int i = 0; i < count; i++) { if (shapes[i].active) printf("Index %d: Shape Type %d\n", i, shapes[i].type); }
        }
    }
    return 0;
}