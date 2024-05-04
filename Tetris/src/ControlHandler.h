#include <puzzlib.h>
#include <functional>
#include <string>

#include "TetrisGame.h"

namespace puzz
{
    class TetrisControlHandler : public Inherit<Visitor, TetrisControlHandler> {
    public:
        TetrisControlHandler(ref_ptr<TetrisGame> game) : m_game(game) {}
        void apply(KeyPressedEvent& event) override {
            //up 265, down 264, left 263, right 262 , space 32
            if (event.GetKey() == 265) {
                int newRotate = (m_game->rotate + 1) % 4;
                if (m_game->move(m_game->x, m_game->y, m_game->rotate, m_game->x, m_game->y, newRotate, m_game->queue[0]))
                {
                    m_game->rotate = newRotate;
                }
            }
            else if (event.GetKey() == 264) {
                if (m_game->move(m_game->x, m_game->y, m_game->rotate, m_game->x, m_game->y + 1, m_game->rotate, m_game->queue[0]))
                {
                    m_game->y += 1;
                }
            }
            else if (event.GetKey() == 263) {
                if (m_game->move(m_game->x, m_game->y, m_game->rotate, m_game->x - 1, m_game->y, m_game->rotate, m_game->queue[0]))
                {
                    m_game->x -= 1;
                }
            }
            else if (event.GetKey() == 262) {
                if (m_game->move(m_game->x, m_game->y, m_game->rotate, m_game->x + 1, m_game->y, m_game->rotate, m_game->queue[0]))
                {
                    m_game->x += 1;
                }
            }
            else if (event.GetKey() == 32) {
                m_game->fallTime += FALL_DELAY * CANVAS_HEIGHT;
            }
            else if (event.GetKey() == 264) {
                m_game->fallTime = FALL_DELAY;
            }
        }
    private:
        ref_ptr<TetrisGame> m_game;
    };
}