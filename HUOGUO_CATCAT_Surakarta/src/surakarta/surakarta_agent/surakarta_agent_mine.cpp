#include "surakarta/surakarta_agent/surakarta_agent_mine.h"
#include <iostream>
#include "surakarta/surakarta_rule_manager.cpp"
#include "surakarta/surakarta_rule_manager.h"
int flagg;                       // black = 0  white = 1;
int weizhi_line_1(int i, int j)  // 确定是在第一条路上的第几个位置
{
    if (i == 1)
        return j;
    if (j == 4)
        return i + 6;
    if (i == 4)
        return 17 - j;
    if (j == 1)
        return 23 - i;
}
int weizhi_line_2(int i, int j)  // 确定是在第二条路上的第几个位置
{
    if (i == 2)
        return j;
    if (j == 3)
        return i + 6;
    if (i == 3)
        return 17 - j;
    if (j == 2)
        return 23 - i;
}
int which_line(int i, int j)  // 确定是哪一条路
{
    int f1 = 0, f2 = 0;
    if ((i == 1) || (i == 4) || (j == 1) || (j == 4))
        f1 = 1;
    if ((i == 2) || (i == 3) || (j == 2) || (j == 3))
        f2 = 2;
    return f1 + f2;
}

int JudgeMove(int move_from_x_my, int move_from_y_my, int move_to_x_my, int move_to_y_my, int qizi[6][6], int player) {
    if (move_from_x_my < 0 || move_from_x_my >= 6 || move_from_y_my < 0 || move_from_y_my >= 6 ||
        move_to_x_my < 0 || move_to_x_my >= 6 || move_to_y_my < 0 || move_to_y_my >= 6) {
        return -10;  // 返回超出棋盘范围的非法移动原因
    }

    if (qizi[move_from_x_my][move_from_y_my] != 0 && qizi[move_from_x_my][move_from_y_my] != 1) {
        return -10;  // 返回移动了一个不是棋子的位置的非法移动原因
    }

    if ((qizi[move_from_x_my][move_from_y_my] == 0 && flagg == 1) || ((qizi[move_from_x_my][move_from_y_my] == 1) && flagg == 0)) {
        return -10;  // 返回移动了not the player's的非法移动原因
    }
    if (qizi[move_to_x_my][move_to_y_my] != 0 && qizi[move_to_x_my][move_to_y_my] != 1) {
        int xf, yf;  // 判断有没有在周围一格内移动
        if (move_from_x_my <= move_to_x_my)
            xf = move_to_x_my - move_from_x_my;
        else
            xf = move_from_x_my - move_to_x_my;
        if (move_from_y_my <= move_to_y_my)
            yf = move_to_y_my - move_from_y_my;
        else
            yf = move_from_y_my - move_to_y_my;

        if (xf <= 1 && yf <= 1)
            return 0;  // 在周围一个内不吃子移动
        else
            return -10;  // 没有在周围一格
    }
    if ((qizi[move_to_x_my][move_to_y_my] == 0 && flagg == 0) || ((qizi[move_to_x_my][move_to_y_my] == 1) && flagg == 1)) {
        return -10;  // eat the same color
    }
    if ((qizi[move_to_x_my][move_to_y_my] == 0 || qizi[move_to_x_my][move_to_y_my] == 1) && (((move_to_x_my == 0) + (move_to_y_my == 0) + (move_to_x_my == 6 - 1) + (move_to_y_my == 6 - 1) == 2) || ((move_from_x_my == 0) + (move_from_y_my == 0) + (move_from_x_my == 6 - 1) + (move_from_y_my == 6 - 1) == 2))) {
        return -10;  // 如果在四个小角落就不能
    }

    int po_from, po_to, line_from, line_to;
    line_from = which_line(move_from_x_my, move_from_y_my);
    line_to = which_line(move_to_x_my, move_to_y_my);
    if (line_from != 3 && line_from != line_to && line_to != 3) {
        return -10;  // 如果要吃的子和移动的子不在一条路上 吃不到
    }
    int line_1[25] = {0}, line_2[25] = {0};

    if (line_from == 1 || line_to == 1 || (line_to == 3 && line_from == 3)) {
        for (int i = 0; i <= 5; i++)  // 读取这一条路上的棋子状态 line【x】 如果有挡路 1 没有棋子 0
        {
            if (qizi[1][i] == 0 || qizi[1][i] == 1) {
                line_1[i] = 1;
            }
            if (qizi[i][4] == 0 || qizi[i][4] == 1) {
                line_1[i + 6] = 1;
            }
            if (qizi[4][5 - i] == 0 || qizi[4][5 - i] == 1) {
                line_1[i + 12] = 1;
            }
            if (qizi[5 - i][1] == 0 || qizi[5 - i][1] == 1) {
                line_1[i + 18] = 1;
            }
        }
        po_from = weizhi_line_1(move_from_x_my, move_from_y_my);
        po_to = weizhi_line_1(move_to_x_my, move_to_y_my);
        int po_to2 = 100;
        if (po_to == 1 || po_to == 4 || po_to == 10 || po_to == 16)  // 关于交叉点处,到达此处可能能有两个标记点数字，到达任意一个即可。
        {
            if (po_to == 4 || po_to == 10 || po_to == 16)
                po_to2 = po_to + 3;
            if (po_to == 1)
                po_to2 = 22;
        }
        // 当正好在旋吃轨迹两端时可以直接吃
        if ((po_from == 0 && po_to == 23) || (po_from == 23 && po_to == 0) || (po_from == 17 && po_to == 18) || (po_from == 11 && po_to == 12) || (po_from == 12 && po_to == 11) || (po_from == 18 && po_to == 17) || (po_from == 5 && po_to == 6) || (po_from == 6 && po_to == 5))
            return 10;
        int ff = 0;
        // 开始向坐标增大方向旋吃
        for (int i = po_from + 1;; i++) {
            if (i == 24)
                i = 0;

            if ((i == po_to || i == po_to2) && ff == 1)
                return 10;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                ff = 1;
            if (line_1[i] != 0)
                break;
        }
        ff = 0;
        // 开始向坐标减小方向旋吃
        for (int i = po_from - 1;; i--) {
            if (i == -1)
                i = 23;

            if ((i == po_to || i == po_to2) && ff == 1)
                return 10;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                ff = 1;
            if (line_1[i] != 0)
                break;
        }
        // 当from也在交叉点时候 from也有两个目标
        if (po_from == 1 || po_from == 4 || po_from == 10 || po_from == 16) {
            if (po_from == 4 || po_from == 10 || po_from == 16)
                po_from = po_from + 3;
            if (po_from == 1)
                po_from = 22;
            ff = 0;
            for (int i = po_from + 1;; i++) {
                if (i == 24)
                    i = 0;

                if ((i == po_to || i == po_to2) && ff == 1)
                    return 10;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    ff = 1;
                if (line_1[i] != 0)
                    break;
            }
            ff = 0;
            for (int i = po_from - 1;; i--) {
                if (i == -1)
                    i = 23;

                if ((i == po_to || i == po_to2) && ff == 1)
                    return 10;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    ff = 1;
                if (line_1[i] != 0)
                    break;
            }
        }
        if (line_from == 1 || line_to == 1)
            return -10;
    }
    if (line_from == 2 || line_to == 2 || (line_to == 3 && line_from == 3)) {
        for (int i = 0; i <= 5; i++) {
            if (qizi[2][i] == 0 || qizi[2][i] == 1) {
                line_2[i] = 1;
            }
            if (qizi[i][3] == 0 || qizi[i][3] == 1) {
                line_2[i + 6] = 1;
            }
            if (qizi[3][5 - i] == 0 || qizi[3][5 - i] == 1) {
                line_2[i + 12] = 1;
            }
            if (qizi[5 - i][2] == 0 || qizi[5 - i][2] == 1) {
                line_2[i + 18] = 1;
            }
        }
        po_from = weizhi_line_2(move_from_x_my, move_from_y_my);
        po_to = weizhi_line_2(move_to_x_my, move_to_y_my);  //???
        int po_to2 = 100;
        if (po_to == 3 || po_to == 9 || po_to == 15 || po_to == 2)  // 3=8 9=14 15=20 21=2
        {
            if (po_to == 3 || po_to == 9 || po_to == 15) {
                po_to2 = po_to + 5;
            }
            if (po_to == 2)
                po_to2 = 21;
        }
        if ((po_from == 0 && po_to == 23) || (po_from == 23 && po_to == 0) || (po_from == 17 && po_to == 18) || (po_from == 11 && po_to == 12) || (po_from == 12 && po_to == 11) || (po_from == 18 && po_to == 17) || (po_from == 5 && po_to == 6) || (po_from == 6 && po_to == 5))
            return 10;
        int ff = 0;
        for (int i = po_from + 1;; i++) {
            if (i == 24)
                i = 0;

            if ((i == po_to || i == po_to2) && ff == 1)
                return 10;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                ff = 1;
            if (line_2[i] != 0)
                break;
        }
        ff = 0;
        for (int i = po_from - 1;; i--) {
            if (i == -1)
                i = 23;

            if ((i == po_to || i == po_to2) && ff == 1)
                return 10;
            if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                ff = 1;
            if (line_2[i] != 0)
                break;
        }
        if (po_from == 3 || po_from == 9 || po_from == 15 || po_from == 2)  // 3=8 9=14 15=20 21=2
        {
            if (po_from == 3 || po_from == 9 || po_from == 15) {
                po_from = po_from + 5;
            }
            if (po_from == 2)
                po_from = 21;
            ff = 0;
            for (int i = po_from + 1;; i++) {
                if (i == 24)
                    i = 0;

                if ((i == po_to || i == po_to2) && ff == 1)
                    return 10;
                if (line_2[i] != 0)
                    break;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    ff = 1;
            }
            ff = 0;
            for (int i = po_from - 1;; i--) {
                if (i == -1)
                    i = 23;

                if ((i == po_to || i == po_to2) && ff == 1)
                    return 10;
                if (line_2[i] != 0)
                    break;
                if (i == 6 || i == 5 || i == 11 || i == 12 || i == 17 || i == 18 || i == 0 || i == 23)
                    ff = 1;
            }
        }

        return -10;
    }
}

int whatI;
int num_sou = 0;
int qizi[6][6] = {2};

int FenShuJudge(int move_from_x_my, int move_from_y_my, int move_to_x_my, int move_to_y_my, int qizi[6][6], int player) {
    if (JudgeMove(move_from_x_my, move_from_y_my, move_to_x_my, move_to_y_my, qizi, player) == -10)
        return -10;
    if (JudgeMove(move_from_x_my, move_from_y_my, move_to_x_my, move_to_y_my, qizi, player) == 10)
        return 10;
    num_sou++;
    if (num_sou++ == 10) {
        num_sou = 0;
        return 0;
    }
    int my_from_x[100], my_from_y[100], ff = 0;
    qizi[move_from_x_my][move_from_y_my] = 2;
    qizi[move_to_x_my][move_to_y_my] = player;
    if (player == 0)
        player = 1;
    else
        player = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if (qizi[i][j] == 0) {
                qizi[i][j] = 0;
                if (player == 0) {
                    my_from_x[ff] = i;
                    my_from_y[ff] = j;
                    ff++;
                }
            }
            if (qizi[i][j] == 1 && player == 1) {
                qizi[i][j] = 1;
                if (flag == 1) {
                    my_from_x[ff] = i;
                    my_from_y[ff] = j;
                    ff++;
                }
            }
        }
    }  // which place I can move from
    int fenshu, fenshumax = -10000, ready_to_x, ready_to_y, ready_from_x, ready_from_y;
    for (int i = 0; i < ff; i++) {
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                if (JudgeMove(my_from_x[i], my_from_y[i], j, k, qizi, player) == -10)
                    continue;
            }
            fenshu = FenShuJudge(my_from_x[i], my_from_y[i], j, k, qizi, player);
            if (fenshu == 10)
                return (-1) * fenshu;
            if (fenshu >= fenshumax) {
                fenshumax = fenshu;
                ready_from_x = my_from_x[i];
                ready_from_y = my_from_y[i];
                ready_to_x = j;
                ready_to_y = k;
            }
        }
    }
    qizi[move_from_x_my][move_from_y_my] = player;
    qizi[move_to_x_my][move_to_y_my] = 2;
    return (-1) * fenshumax;
}

SurakartaMove SurakartaAgentMine::CalculateMove() {
    // TODO: Implement your own ai here.
    int flag;
    SurakartaPlayer current_player = game_info_->current_player_;
    if (current_player == SurakartaPlayer::BLACK) {
        flag = 0;
        whatI = 0;  // black player's turn
    } else if (current_player == SurakartaPlayer::WHITE) {
        flag = 1;
        whatI = 1;  // white player's turn
    }
    int my_from_x[100], my_from_y[100], ff = 0;
    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            if ((*board_)[i][j]->GetColor() == PieceColor::BLACK) {
                qizi[i][j] = 0;
                if (flag == 0) {
                    my_from_x[ff] = i;
                    my_from_y[ff] = j;
                    ff++;
                }
            }
            if ((*board_)[i][j]->GetColor() == PieceColor::WHITE && flag == 1) {
                qizi[i][j] = 1;
                if (flag == 1) {
                    my_from_x[ff] = i;
                    my_from_y[ff] = j;
                    ff++;
                }
            }
        }
    }  // which place I can move from
    int fenshu, fenshumax = -10000, ready_to_x, ready_to_y, ready_from_x, ready_from_y;
    for (int i = 0; i < ff; i++) {
        mymove.from = SurakartaPosition(my_from_x[i], my_from_y[i]);
        for (int j = 0; j < 6; j++) {
            for (int k = 0; k < 6; k++) {
                if (JudgeMove(my_from_x[i], my_from_y[i], j, k, qizi, player) == -10)
                    continue;
                fenshu = FenShuJudge(my_from_x[i], my_from_y[i], j, k, qizi, flag);
                if (fenshu >= fenshumax) {
                    fenshumax = fenshu;
                    ready_from_x = my_from_x[i];
                    ready_from_y = my_from_y[i];
                    ready_to_x = j;
                    ready_to_y = k;
                }
            }
        }
    }  // which place I can move to

    return SurakartaMove({ready_from_x, ready_from_y}, {ready_to_x, ready_to_y}, game_info_->current_player_);
}