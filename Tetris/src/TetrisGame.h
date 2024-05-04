#pragma once

#include "TetrisBlock.h"
#include <vector>
#include <random>
#include <puzzlib.h>

#define CANVAS_WIDTH 10
#define CANVAS_HEIGHT 20
#define FALL_DELAY 60

class TetrisGame :public puzz::Inherit<puzz::RuntimeModule,TetrisGame>{
public:
    TetrisGame() {
        reset();
    }

    void reset() {
        for (int i = 0; i < CANVAS_HEIGHT; i++) {
            for (int j = 0; j < CANVAS_WIDTH; j++) {
                canvas[i][j].resetBlock();
            }
        }

        for (int i = 0; i < 4; i++) {
            queue[i] = getRandomShape();
        }

        score = 0;
        x = CANVAS_WIDTH / 2;
        y = 0;
        rotate = 0;
        fallTime = 0;
    }

    void startUp() override {
        reset();
    }

    void shutDown() override {
    }

    // 60 FPS
    void Tick(double deltaTime) override {

        fallTime += 1; // Adjust the fall speed as needed

        while (fallTime >= FALL_DELAY) { // Adjust the fall delay as needed
            fallTime -= FALL_DELAY;

            if (move(x, y, rotate, x, y + 1, rotate, queue[0])) {
                y++;
            }
            else {
                score += clearLines();

                x = CANVAS_WIDTH / 2;
                y = 0;
                rotate = 0;
                fallTime = 0;
                queue[0] = queue[1];
                queue[1] = queue[2];
                queue[2] = queue[3];
                queue[3] = getRandomShape();

                if (!move(x, y, rotate, x, y, rotate, queue[0])) {
                    reset();
                }
            }
        }
    }

    bool move(int originalX, int originalY, int originalRotate, int newX, int newY, int newRotate, ShapeId shapeId) {
        Shape shapeData = shapes[static_cast<int>(shapeId)];
        int size = shapeData.size;

        // Check if the new position is valid to place the block
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (shapeData.rotates[newRotate][i][j]) {
                    if (newX + j < 0 || newX + j >= CANVAS_WIDTH || newY + i < 0 || newY + i >= CANVAS_HEIGHT) {
                        return false;
                    }
                    if (!canvas[newY + i][newX + j].current && canvas[newY + i][newX + j].shape != ShapeId::EMPTY) {
                        return false;
                    }
                }
            }
        }

        // Remove the old position
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (shapeData.rotates[originalRotate][i][j]) {
                    canvas[originalY + i][originalX + j].resetBlock();
                }
            }
        }

        // Move the block to the new position
        for (int i = 0; i < size; i++) {
            for (int j = 0; j < size; j++) {
                if (shapeData.rotates[newRotate][i][j]) {
                    canvas[newY + i][newX + j].setBlock(shapeData.color, shapeId, true);
                }
            }
        }

        return true;
    }

    int clearLines() {
        for (int i = 0; i < CANVAS_HEIGHT; i++) {
            for (int j = 0; j < CANVAS_WIDTH; j++) {
                if (canvas[i][j].current) {
                    canvas[i][j].current = false;
                }
            }
        }

        int linesCleared = 0;

        for (int i = CANVAS_HEIGHT - 1; i >= 0; i--) {
            bool isFull = true;
            for (int j = 0; j < CANVAS_WIDTH; j++) {
                if (canvas[i][j].shape == ShapeId::EMPTY) {
                    isFull = false;
                    break;
                }
            }

            if (isFull) {
                linesCleared++;

                for (int j = i; j > 0; j--) {
                    for (int k = 0; k < CANVAS_WIDTH; k++) {
                        canvas[j][k] = canvas[j - 1][k];
                    }
                }

                for (int k = 0; k < CANVAS_WIDTH; k++) {
                    canvas[0][k].resetBlock();
                }

                i++;
            }
        }

        return linesCleared;
    }

    ShapeId getRandomShape() {
        std::random_device rd;
        std::mt19937 rng(rd());
        std::uniform_int_distribution<int> dist(0, 6);
        return static_cast<ShapeId>(dist(rng));
    }

    glm::vec4 getColor(int x, int y) {
        return canvas[y][x].getColor();
    }

    TetrisBlock canvas[CANVAS_HEIGHT][CANVAS_WIDTH];
    ShapeId queue[4];
    int score;
    int x;
    int y;
    int rotate;
    int fallTime;
};