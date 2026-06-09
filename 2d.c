#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define W 60
#define H 20

typedef enum { LINE, RECT, TRI, CIRCLE } Type;

typedef struct {
    int id;
    Type type;
    int d[6];
} Shape;

char canvas[H][W];
Shape shapes[100];
int count = 0;
int id_counter = 1;

void clear_canvas() {
    for (int y = 0; y < H; y++) {
        for (int x = 0; x < W; x++) {
            canvas[y][x] = '_';
        }
    }
}

void plot(int x, int y) {
    if (x >= 0 && x < W && y >= 0 && y < H) {
        canvas[y][x] = '*';
    }
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
        if (d > 0) {
            y--;
            d = d + 4 * (x - y) + 10;
        } else {
            d = d + 4 * x + 6;
        }
    }
}

void display() {
    clear_canvas();
    for (int i = 0; i < count; i++) {
        Shape s = shapes[i];
        if (s.type == LINE) {
            draw_line(s.d[0], s.d[1], s.d[2], s.d[3]);
        } else if (s.type == RECT) {
            int x = s.d[0], y = s.d[1], w = s.d[2], h = s.d[3];
            draw_line(x, y, x + w - 1, y);
            draw_line(x, y + h - 1, x + w - 1, y + h - 1);
            draw_line(x, y, x, y + h - 1);
            draw_line(x + w - 1, y, x + w - 1, y + h - 1);
        } else if (s.type == TRI) {
            draw_line(s.d[0], s.d[1], s.d[2], s.d[3]);
            draw_line(s.d[2], s.d[3], s.d[4], s.d[5]);
            draw_line(s.d[4], s.d[5], s.d[0], s.d[1]);
        } else if (s.type == CIRCLE) {
            draw_circle(s.d[0], s.d[1], s.d[2]);
        }
    }

    // New cleanly spaced X-Axis Labels
    printf("\n    ");
    for (int x = 0; x < W; x++) {
        if (x % 10 == 0) {
            printf("%-10d", x); // Prints 0, 10, 20 with exact padding space
        }
    }
    printf("\n    ");
    for (int x = 0; x < W; x++) {
        if (x % 5 == 0) {
            printf("|"); // Mark visual indicators every 5 steps
        } else {
            printf(" ");
        }
    }
    printf("\n");

    for (int y = 0; y < H; y++) {
        printf("%2d |", y); // Added a clean divider line for Y axis
        for (int x = 0; x < W; x++) {
            putchar(canvas[y][x]);
        }
        printf("\n");
    }
}

void add_shape(Type type, int d0, int d1, int d2, int d3, int d4, int d5) {
    if (count >= 100) return;
    shapes[count].id = id_counter++;
    shapes[count].type = type;
    shapes[count].d[0] = d0; shapes[count].d[1] = d1; shapes[count].d[2] = d2;
    shapes[count].d[3] = d3; shapes[count].d[4] = d4; shapes[count].d[5] = d5;
    count++;
}

void delete_shape(int id) {
    int idx = -1;
    for (int i = 0; i < count; i++) {
        if (shapes[i].id == id) {
            idx = i;
            break;
        }
    }
    if (idx != -1) {
        for (int i = idx; i < count - 1; i++) {
            shapes[i] = shapes[i + 1];
        }
        count--;
    }
}

void modify_shape(int id, int d0, int d1, int d2, int d3, int d4, int d5) {
    for (int i = 0; i < count; i++) {
        if (shapes[i].id == id) {
            shapes[i].d[0] = d0; shapes[i].d[1] = d1; shapes[i].d[2] = d2;
            shapes[i].d[3] = d3; shapes[i].d[4] = d4; shapes[i].d[5] = d5;
            break;
        }
    }
}

int main() {
    int choice, id;
    int d0, d1, d2, d3, d4, d5;

    while (1) {
        display();
        printf("\n--- Object Registry ---\n");
        for (int i = 0; i < count; i++) {
            Shape s = shapes[i];
            if (s.type == LINE) {
                printf("ID: %d | Type: Line | Coordinates: [x0=%d, y0=%d, x1=%d, y1=%d]\n", s.id, s.d[0], s.d[1], s.d[2], s.d[3]);
            } else if (s.type == RECT) {
                printf("ID: %d | Type: Rectangle | Coordinates: [x=%d, y=%d, width=%d, height=%d]\n", s.id, s.d[0], s.d[1], s.d[2], s.d[3]);
            } else if (s.type == TRI) {
                printf("ID: %d | Type: Triangle | Coordinates: [x0=%d, y0=%d, x1=%d, y1=%d, x2=%d, y2=%d]\n", s.id, s.d[0], s.d[1], s.d[2], s.d[3], s.d[4], s.d[5]);
            } else if (s.type == CIRCLE) {
                printf("ID: %d | Type: Circle | Coordinates: [center_x=%d, center_y=%d, radius=%d]\n", s.id, s.d[0], s.d[1], s.d[2]);
            }
        }

        printf("\n1. Add Line\n2. Add Rect\n3. Add Tri\n4. Add Circle\n5. Modify\n6. Delete\n7. Exit\nChoice: ");
        if (scanf("%d", &choice) != 1) break;

        if (choice == 1) {
            printf("Enter x0 y0 x1 y1: ");
            scanf("%d %d %d %d", &d0, &d1, &d2, &d3);
            add_shape(LINE, d0, d1, d2, d3, 0, 0);
        } else if (choice == 2) {
            printf("Enter x y w h: ");
            scanf("%d %d %d %d", &d0, &d1, &d2, &d3);
            add_shape(RECT, d0, d1, d2, d3, 0, 0);
        } else if (choice == 3) {
            printf("Enter x0 y0 x1 y1 x2 y2: ");
            scanf("%d %d %d %d %d %d", &d0, &d1, &d2, &d3, &d4, &d5);
            add_shape(TRI, d0, d1, d2, d3, d4, d5);
        } else if (choice == 4) {
            printf("Enter xc yc r: ");
            scanf("%d %d %d", &d0, &d1, &d2);
            add_shape(CIRCLE, d0, d1, d2, 0, 0, 0);
        } else if (choice == 5) {
            printf("Enter Shape ID to modify: ");
            scanf("%d", &id);
            
            int found_type = -1;
            for(int i = 0; i < count; i++) {
                if(shapes[i].id == id) {
                    found_type = shapes[i].type;
                    break;
                }
            }

            if (found_type == LINE) {
                printf("Enter new x0 y0 x1 y1: ");
                scanf("%d %d %d %d", &d0, &d1, &d2, &d3);
                modify_shape(id, d0, d1, d2, d3, 0, 0);
            } else if (found_type == RECT) {
                printf("Enter new x y w h: ");
                scanf("%d %d %d %d", &d0, &d1, &d2, &d3);
                modify_shape(id, d0, d1, d2, d3, 0, 0);
            } else if (found_type == TRI) {
                printf("Enter new x0 y0 x1 y1 x2 y2: ");
                scanf("%d %d %d %d %d %d", &d0, &d1, &d2, &d3, &d4, &d5);
                modify_shape(id, d0, d1, d2, d3, d4, d5);
            } else if (found_type == CIRCLE) {
                printf("Enter new xc yc r: ");
                scanf("%d %d %d", &d0, &d1, &d2);
                modify_shape(id, d0, d1, d2, 0, 0, 0);
            } else {
                printf("Invalid Shape ID!\n");
            }
        } else if (choice == 6) {
            printf("Enter Shape ID to delete: ");
            scanf("%d", &id);
            delete_shape(id);
        } else if (choice == 7) {
            break;
        }
    }
    return 0;
}