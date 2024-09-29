//MD AKASH HOSSAIN
//2021521460115


#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <windows.h>
#include <time.h>

struct Ball {
    int x, y;
    int vx, vy;
    int r;
    bool active;
};

int main() {
    srand((unsigned int)time(NULL)); 

    int r = 10; 
    Ball balls[2]; 
    balls[0] = { 300, 580, 5, -6, r, true }; // Main ball
    balls[1] = { 0, 0, 0, 0, r, false }; // Second ball (inactive at start)

    int i, j;
    int w = 90, h = 40; 
    int xs, ys;
    int gap = 5;

    int bx = 250;  
    int len = 100; 
    int th = 10;   
    bool ls = true; 
    int miss_count = 0; 
    bool game_over = false; 

  
    bool elongate_board = false;
    bool slow_down = false;
    clock_t power_up_start = 0;

   
    bool gift_bag_active = false;
    int gift_bag_x, gift_bag_y;

  
    bool rect_hit[6][5] = { false };


    COLORREF brickColors[6][5] = {
        { RGB(255, 0, 0), RGB(255, 165, 0), RGB(255, 255, 0), RGB(0, 255, 0), RGB(0, 0, 255) },
        { RGB(128, 0, 128), RGB(255, 105, 180), RGB(75, 0, 130), RGB(0, 255, 255), RGB(139, 69, 19) },
        { RGB(173, 255, 47), RGB(255, 99, 71), RGB(138, 43, 226), RGB(240, 230, 140), RGB(0, 191, 255) },
        { RGB(127, 255, 212), RGB(219, 112, 147), RGB(128, 128, 0), RGB(255, 228, 181), RGB(147, 112, 219) },
        { RGB(72, 61, 139), RGB(106, 90, 205), RGB(0, 206, 209), RGB(176, 224, 230), RGB(139, 0, 139) },
        { RGB(255, 20, 147), RGB(0, 100, 0), RGB(220, 20, 60), RGB(218, 112, 214), RGB(105, 105, 105) }
    };

    initgraph(600, 800); 

    setbkcolor(RGB(200, 200, 200)); 
    cleardevice();

    BeginBatchDraw();
    while (1) {
        if (game_over) {
            settextcolor(RED);
            outtextxy(200, 300, _T("Game Over!"));
            outtextxy(180, 350, _T("To restart press Enter."));
            outtextxy(180, 380, _T("Press ESC to exit."));

            if (GetAsyncKeyState(VK_RETURN) < 0) {
                balls[0] = { 300, 580, 5, -6, r, true };
                balls[1].active = false;
                miss_count = 0;
                game_over = false;
                ls = true;
                elongate_board = false;
                slow_down = false;
                for (i = 0; i < 6; i++) {
                    for (j = 0; j < 5; j++) {
                        rect_hit[i][j] = false;
                    }
                }
                Sleep(150);
            }
            else if (GetAsyncKeyState(VK_ESCAPE) < 0) {
                break;
            }

            FlushBatchDraw();
            Sleep(100);
            cleardevice();
            continue;
        }

        
        for (i = 0; i < 6; i++) {
            for (j = 0; j < 5; j++) {
                if (!rect_hit[i][j]) {
                    xs = i * w;
                    ys = j * h;
                    setfillcolor(brickColors[i][j]);
                    fillrectangle(gap * (i + 1) + xs, gap * (j + 1) + ys,
                        gap * (i + 1) + xs + w, gap * (j + 1) + ys + h);
                }
            }
        }

        // Draw the board with a color
        setfillcolor(RGB(50, 0, 50)); 
        fillrectangle(bx, 760, bx + len, 760 + th);

        // Generate and draw gift bag
        if (!gift_bag_active && rand() % 500 == 0) {
            gift_bag_x = rand() % (600 - 20) + 10;
            gift_bag_y = rand() % (400) + 100;
            gift_bag_active = true;
        }
        if (gift_bag_active) {
            setfillcolor(RGB(255, 215, 0)); 
            fillrectangle(gift_bag_x, gift_bag_y, gift_bag_x + 20, gift_bag_y + 20);
        }

        // Ball behavior
        if (ls) {
            setfillcolor(RGB(128, 0, 0)); // Red ball before game starts
            fillcircle(bx + len / 2, 760 - r, r);

            if (GetAsyncKeyState(VK_SPACE) < 0) {
                ls = false;
                Sleep(150);
            }
        }
        else {
            for (int k = 0; k < 2; k++) {
                if (!balls[k].active) continue;

                setfillcolor(RGB(0, 128, 0)); 
                balls[k].x += balls[k].vx;
                balls[k].y += balls[k].vy;

               
                if (balls[k].y + balls[k].r >= 760) {
                    if (balls[k].x + balls[k].r > bx && balls[k].x - balls[k].r < bx + len) {
                        balls[k].vy = -balls[k].vy;
                        balls[k].y = 760 - balls[k].r;
                    }
                    else {
                        balls[k].active = false;
                        miss_count++;
                        // Display "Lose" message immediately when the ball misses
                        settextcolor(RED);
                        settextstyle(60, 0, _T("Arial"));
                        outtextxy(250, 400, _T("Lose"));
                        game_over = true; // Set game over state
                        FlushBatchDraw();
                        Sleep(2000); // Pause to show message for 2 seconds
                        game_over = true; 
                    }
                }
                else if (balls[k].y - balls[k].r <= 0) {
                    balls[k].vy = -balls[k].vy;
                    balls[k].y = balls[k].r;
                }

                
                if (balls[k].x - balls[k].r <= 0 || balls[k].x + balls[k].r >= 600) {
                    balls[k].vx = -balls[k].vx;
                    balls[k].x = (balls[k].x - balls[k].r <= 0) ? balls[k].r : 600 - balls[k].r;
                }

                fillcircle(balls[k].x, balls[k].y, balls[k].r);

               
                if (gift_bag_active &&
                    balls[k].x > gift_bag_x && balls[k].x < gift_bag_x + 20 &&
                    balls[k].y > gift_bag_y && balls[k].y < gift_bag_y + 20) {
                    gift_bag_active = false;
                    elongate_board = true;
                    slow_down = true;
                    power_up_start = clock();
                    len = 200; 
                    balls[1] = { balls[0].x, balls[0].y, -balls[0].vx, balls[0].vy, r, true }; // Activate second ball
                }
            }

            // Power-up duration (5 seconds)
            if (elongate_board && (clock() - power_up_start) >= 5000) {
                elongate_board = false;
                slow_down = false;
                len = 100; 
            }
        }

       
        for (int k = 0; k < 2; k++) {
            if (!balls[k].active) continue;

            for (i = 0; i < 6; i++) {
                for (j = 0; j < 5; j++) {
                    xs = i * w;
                    ys = j * h;
                    if (!rect_hit[i][j] &&
                        balls[k].x + balls[k].r >= gap * (i + 1) + xs &&
                        balls[k].x - balls[k].r <= gap * (i + 1) + xs + w &&
                        balls[k].y + balls[k].r >= gap * (j + 1) + ys &&
                        balls[k].y - balls[k].r <= gap * (j + 1) + ys + h) {

                        rect_hit[i][j] = true;
                        balls[k].vy = -balls[k].vy; 
                    }
                }
            }
        }

        // Move the board with left and right arrow keys
        if (GetAsyncKeyState(VK_LEFT) < 0) bx -= 20;
        if (GetAsyncKeyState(VK_RIGHT) < 0) bx += 20;
        if (bx <= 0) bx = 0;
        if (bx + len >= 600) bx = 600 - len;

        FlushBatchDraw();
        Sleep(slow_down ? 60 : 30);
        cleardevice();
    }
    EndBatchDraw();

    closegraph();
    return 0;
}