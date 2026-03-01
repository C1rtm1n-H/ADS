#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

int n, m, l;
int *statements;
int *is_wolf;
bool is_valid = false;

void solve(int index, int wolf_count){
    if(is_valid) return;    // Solution already found
    if(index < (m - wolf_count)){   // Not enough positions left to place wolves
        return;
    }

    if(wolf_count == m){
        for(int i=1; i<=index; i++){
            is_wolf[i] = 0;   // Mark remaining as humans
        }
        checkAndPrint();
        return;
    }

    if(index < 1){  // No more positions left
        return;
    }

    // Try placing a wolf at the current index
    is_wolf[index] = 1;
    solve(index - 1, wolf_count + 1);
    if(is_valid) return;

    // Try placing a human at the current index
    is_wolf[index] = 0;
    solve(index - 1, wolf_count);
    if(is_valid) return;
}

void checkAndPrint(){
    int total_liars = 0;
    int wolf_liars = 0;

    for(int i=1; i<=n; i++){
        int cur_ind = statements[i] > 0 ? statements[i] : -statements[i];   // Absolute index
        int cur_type = statements[i] > 0 ? 0 : 1;   // 0 for human, 1 for wolf

        bool is_true_statement = (is_wolf[cur_ind] == cur_type);
        bool is_liar = false;

        if(is_wolf[i] == 1){
            if(!is_true_statement){
                is_liar = true;
            }
        }else{
            if(!is_true_statement){
                is_liar = true;
            }
        }

        if(is_liar){
            total_liars++;
            if(is_wolf[i] == 1){
                wolf_liars++;
            }
        }
    }

    if(total_liars == l && wolf_liars > 0 && wolf_liars < m){
        is_valid = true;
        bool first = true;
        for(int i=n; i>=1; i--){
            if(is_wolf[i] == 1){
                if(!first){
                    printf(" ");
                }
                printf("%d", i);
                first = false;
            }
        }
        printf("\n");
    }
}

int main(){
    scanf("%d %d %d", &n, &m, &l);
    statements = (int*)malloc((n+1) * sizeof(int));
    is_wolf = (int*)malloc((n+1) * sizeof(int));

    for(int i=1; i<=n; i++){
        scanf("%d", &statements[i]);
    }

    solve(n, 0);

    if(!is_valid){
        printf("No Solution\n");
    }

    free(statements);
    free(is_wolf);
    return 0;
}