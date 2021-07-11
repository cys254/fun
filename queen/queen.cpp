#include <stdlib.h>
#include <stdio.h>

int* create_board(int size) {
    int *board = (int*)malloc(sizeof(int)*size);
    for (int i = 0; i < size; i++) {
        board[i] = i+1;
    }
    return board;
}

int check_conflict(int *board, int size, int index) {
    for (int i = 0; i < index; i++) {
        int diff = board[index] - board[i];
        if (diff < 0) diff = -diff;
        if (diff == index - i) return 1;
    }
    return 0;
}

void permute(int *board, int size, int index) {
    if (index >= 2) {
        if (check_conflict(board, size, index-1)) return;
    }
    if (index == size-1) {
        if (check_conflict(board, size, size-1)) return;
//        for (int i = 0; i < size; i++) {
//            printf(" %d", board[i]);
//        }
//        printf("\n");
        return;
    }
    permute(board, size, index+1);
    for (int i = index+1; i < size; i++) {
        int tmp = board[index];
        board[index] = board[i];
        board[i] = tmp;
        permute(board, size, index+1);
    }
    int tmp = board[index];
    for (int i = index+1; i < size; i++) {
        board[i-1] = board[i];
    }
    board[size-1] = tmp;
}

int main(int argc, char** argv) {
    int size = 3;
    if (argc > 1) {
        size = atoi(argv[1]);
    }
    int *board = create_board(size);
    permute(board, size, 0);
}
