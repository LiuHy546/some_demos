#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>
#include <ctype.h>
#include <errno.h>

#define MAX_HISTORY 100
#define MAX_LINE_LEN 1024

typedef struct {
    char buf[MAX_LINE_LEN];
    int length;
    int cursorPos;
    int historyPos;  // -1表示不在浏览历史记录
    char currentLine[MAX_LINE_LEN];  // 保存当前正在编辑的行
    int isFromHistory;  // 标记当前内容是否来自历史记录
} LineState;

static struct termios g_origTermios;
static char g_history[MAX_HISTORY][MAX_LINE_LEN];
static int g_historyCount = 0;
static int g_isTerminal = 0;

// 检查是否在终端环境中运行
int CheckTerminal() {
    return isatty(STDIN_FILENO) && isatty(STDOUT_FILENO);
}

// 设置终端为原始模式
int EnableRawMode() {
    if (!g_isTerminal) return -1;
    
    if (tcgetattr(STDIN_FILENO, &g_origTermios) == -1) {
        perror("tcgetattr");
        return -1;
    }

    struct termios raw = g_origTermios;
    raw.c_lflag &= ~(ECHO | ICANON | ISIG | IEXTEN);
    raw.c_iflag &= ~(IXON | ICRNL | BRKINT | INPCK | ISTRIP);
    raw.c_cflag &= ~(CSIZE | PARENB);
    raw.c_cflag |= CS8;
    raw.c_oflag &= ~(OPOST);
    raw.c_cc[VMIN] = 1;
    raw.c_cc[VTIME] = 0;

    if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1) {
        perror("tcsetattr");
        return -1;
    }
    return 0;
}

// 恢复终端原始设置
void DisableRawMode() {
    if (g_isTerminal) {
        tcsetattr(STDIN_FILENO, TCSAFLUSH, &g_origTermios);
    }
}

// 清除并重绘当前行
void RefreshLine(const LineState *state, const char *prompt) {
    if (!g_isTerminal) return;
    int promptLen = strlen(prompt);
    int bufLen = state->length;

    // 1. 回到行首 + 清除整行（关键修正：用\033[2K替代\033[K）
    printf("\r\033[2K");

    // 2. 输出提示符和当前输入内容
    printf("%s", prompt);
    fwrite(state->buf, 1, bufLen, stdout);

    // 3. 计算光标绝对位置（提示符长度 + 光标在缓冲区中的位置）
    int cursorAbsPos = promptLen + state->cursorPos;
    // 强制光标移动到正确位置（无论是否在行尾）
    printf("\r\033[%dC", cursorAbsPos);

    fflush(stdout); // 确保所有终端指令立即执行
}

// 处理上箭头键 - 从最新开始显示历史记录
void HandleUpArrow(LineState *state) {
    if (g_historyCount == 0) return;

    // 如果是第一次按上键，保存当前编辑的行
    if (state->historyPos == -1) {
        strncpy(state->currentLine, state->buf, MAX_LINE_LEN);
        state->historyPos = g_historyCount - 1;  // 从最新记录开始
    } else if (state->historyPos > 0) {
        state->historyPos--;  // 移动到更早的记录
    }

    // 加载对应的历史记录
    const char *historyItem = g_history[state->historyPos];
    strncpy(state->buf, historyItem, MAX_LINE_LEN);
    state->length = strlen(state->buf);
    state->cursorPos = state->length;
    state->isFromHistory = 1;
}

// 处理下箭头键 - 向最新方向移动
void HandleDownArrow(LineState *state) {
    if (state->historyPos == -1 || g_historyCount == 0) return;

    state->historyPos++;  // 移动到更新的记录
    
    if (state->historyPos >= g_historyCount) {
        // 超过最新记录，恢复到之前编辑的行
        strncpy(state->buf, state->currentLine, MAX_LINE_LEN);
        state->length = strlen(state->buf);
        state->historyPos = -1;
        state->isFromHistory = 0;
    } else {
        // 加载对应的历史记录
        const char *historyItem = g_history[state->historyPos];
        strncpy(state->buf, historyItem, MAX_LINE_LEN);
        state->length = strlen(state->buf);
        state->isFromHistory = 1;
    }
    state->cursorPos = state->length;
}

// 在历史记录中查找命令
int FindInHistory(const char *cmd) {
    for (int i = 0; i < g_historyCount; i++) {
        if (strcmp(g_history[i], cmd) == 0) {
            return i;
        }
    }
    return -1;
}

// 更新历史记录（保证唯一性，最新命令在数组末尾）
void UpdateHistory(const char *line) {
    if (line == NULL || line[0] == '\0') return;
    
    int foundIdx = FindInHistory(line);
    
    if (foundIdx != -1) {
        // 已存在，移动到最末尾（最新位置）
        char temp[MAX_LINE_LEN];
        strcpy(temp, g_history[foundIdx]);
        
        for (int i = foundIdx; i < g_historyCount - 1; i++) {
            strcpy(g_history[i], g_history[i+1]);
        }
        
        strcpy(g_history[g_historyCount - 1], temp);
    } else {
        // 不存在，添加到最末尾
        if (g_historyCount >= MAX_HISTORY) {
            // 历史记录已满，移除最早的一条
            for (int i = 0; i < g_historyCount - 1; i++) {
                strcpy(g_history[i], g_history[i+1]);
            }
            g_historyCount--;
        }
        
        strcpy(g_history[g_historyCount], line);
        g_historyCount++;
    }
}

// 读取一行输入
char* RobustReadLine(const char* prompt) {
    LineState state = {0};
    state.historyPos = -1;
    state.isFromHistory = 0;
    state.currentLine[0] = '\0';
    
    if (EnableRawMode() == -1) {
        // 无法启用原始模式，使用简单输入方式
        printf("%s", prompt);
        fflush(stdout);
        
        char *input = malloc(MAX_LINE_LEN);
        if (fgets(input, MAX_LINE_LEN, stdin)) {
            input[strcspn(input, "\n")] = '\0';  // 移除换行符
            if (strlen(input) > 0) {
                UpdateHistory(input);
            }
            return input;
        }
        free(input);
        return NULL;
    }
    
    printf("%s", prompt);
    fflush(stdout);
    
    while (1) {
        char c;
        if (read(STDIN_FILENO, &c, 1) <= 0) {
            if (errno == EINTR) continue;
            break;
        }
        
        // 处理普通可打印字符
        if (isprint(c)) {
            if (state.length < MAX_LINE_LEN - 1) {
                if (state.cursorPos < state.length) {
                    memmove(state.buf + state.cursorPos + 1, 
                           state.buf + state.cursorPos, 
                           state.length - state.cursorPos);
                }
                state.buf[state.cursorPos] = c;
                state.length++;
                state.cursorPos++;
                
                // 输入字符时退出历史浏览模式
                if (state.historyPos != -1) {
                    state.historyPos = -1;
                    state.isFromHistory = 0;
                }
                
                RefreshLine(&state, prompt);
            }
        }
        // 退格键
        else if (c == 127) {
            if (state.cursorPos > 0 && state.length > 0) {
                memmove(state.buf + state.cursorPos - 1, 
                       state.buf + state.cursorPos, 
                       state.length - state.cursorPos);
                state.length--;
                state.cursorPos--;
                state.buf[state.length] = '\0';
                
                if (state.historyPos != -1) {
                    state.historyPos = -1;
                    state.isFromHistory = 0;
                }
                
                RefreshLine(&state, prompt);
            }
        }
        // ESC序列(方向键)
        else if (c == 27) {
            char seq[2];
            if (read(STDIN_FILENO, &seq[0], 1) <= 0 || read(STDIN_FILENO, &seq[1], 1) <= 0) break;
            
            if (seq[0] == '[') {
                switch (seq[1]) {
                    case 'D':  // 左箭头
                        if (state.cursorPos > 0) {
                            state.cursorPos--;
                            RefreshLine(&state, prompt);
                        }
                        break;
                    case 'C':  // 右箭头
                        if (state.cursorPos < state.length) {
                            state.cursorPos++;
                            RefreshLine(&state, prompt);
                        }
                        break;
                    case 'A':  // 上箭头
                        HandleUpArrow(&state);
                        RefreshLine(&state, prompt);
                        break;
                    case 'B':  // 下箭头
                        HandleDownArrow(&state);
                        RefreshLine(&state, prompt);
                        break;
                }
            }
        }
        // 回车键
        else if (c == '\n' || c == '\r') {
            // 回车前刷新一遍行，确保光标在行尾
            RefreshLine(&state, prompt);
            printf("\n");
            break;
        }
    }
    
    DisableRawMode();
    
    // 无论是否来自历史记录，执行后都更新历史记录
    if (state.length > 0) {
        UpdateHistory(state.buf);
    }
    
    char *result = strdup(state.buf);
    return result;
}

void InitReadline() {
    g_isTerminal = CheckTerminal();
    if (!g_isTerminal) {
        printf("注意：当前不是终端环境，某些功能可能受限\n");
    }
}

// int main() {
//     g_isTerminal = CheckTerminal();
    
//     if (!g_isTerminal) {
//         printf("注意：当前不是终端环境，某些功能可能受限\n");
//     }
    
//     printf("修复版 Readline 测试 (输入 'exit' 退出)\n");
    
//     // 添加测试历史记录（按时间顺序，最早的在最前面）
//     strcpy(g_history[g_historyCount++], "oldest command");
//     strcpy(g_history[g_historyCount++], "middle command");
//     strcpy(g_history[g_historyCount++], "newest command");
    
//     while (1) {
//         char* input = RobustReadLine("> ");
//         if (!input) break;
        
//         if (strcmp(input, "exit") == 0) {
//             free(input);
//             break;
//         }
        
//         printf("你输入了: %s\n", input);
//         free(input);
//     }
    
//     // 打印历史记录用于调试
//     printf("\n最终历史记录内容(最新在最后):\n");
//     for (int i = 0; i < g_historyCount; i++) {
//         printf("%d: %s\n", i, g_history[i]);
//     }
    
//     return 0;
// }
