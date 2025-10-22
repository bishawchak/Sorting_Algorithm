#include "raylib.h"
#include<stdlib.h>
#include<time.h>
using namespace std;

#define NUM_BARS 50
#define WINDOWS_WIDTH 800
#define WINDOWS_HEIGHT 450

typedef struct Bar{
    int height;
    Color color;
} Bar;

Bar bars[NUM_BARS];

void InitBars(){
    for(int i = 0; i < NUM_BARS; i++){
        bars[i].height = rand()%WINDOWS_HEIGHT;
        bars[i].color = LIGHTGRAY;
    }
}

typedef enum {BUBBLE, SELECTION, INSERTION, QUICK, MERGE, HEAP, SHELL, COUNTING} SortAlgorithm;
char sortNames[][15] = {"BUBBLE", "SELECTION", "INSERTION", "QUICK", "MERGE", "HEAP", "SHELL", "COUNTING"};
SortAlgorithm currentAlgorithm = BUBBLE;
SortAlgorithm lastAlgorithm = BUBBLE;

void DrawBars(){
    for(int i = 0; i < NUM_BARS; i++){
        DrawRectangle(i*(WINDOWS_WIDTH/NUM_BARS),
        WINDOWS_HEIGHT - bars[i].height,
        (WINDOWS_WIDTH / NUM_BARS) - 2,
        bars[i].height,
        bars[i].color);
    }
}

void BubbleSortStep(){
    static int i = 0, j = 0;
    static bool sorted = false;
    
    if(lastAlgorithm != currentAlgorithm){
        i = 0;
        j = 0;
        sorted = false;
    }
    
    if(sorted){
        DrawBars();
        return;
    }
    
    if (i < NUM_BARS - 1) {
        if (j < NUM_BARS - i - 1) {
            bars[j].color = RED;
            bars[j + 1].color = RED;

            if (bars[j].height > bars[j + 1].height) {
                int temp = bars[j].height;
                bars[j].height = bars[j + 1].height;
                bars[j + 1].height = temp;
            }

            bars[j].color = LIGHTGRAY;
            bars[j + 1].color = LIGHTGRAY;
            j++;
        } else {
            j = 0;
            i++;
        }
    } else {
        sorted = true;
        i = 0;
        j = 0;
    }
    DrawBars();
}

void SelectionSortStep(){
    static int i = 0, j = 0, minIndex = 0;
    static bool sorted = false;
    
    if(lastAlgorithm != currentAlgorithm){
        i = 0;
        j = 0;
        minIndex = 0;
        sorted = false;
    }
    
    if(sorted){
        DrawBars();
        return;
    }
    
    if(i < NUM_BARS - 1){
        if(j < NUM_BARS){
            bars[minIndex].color = BLUE;
            bars[j].color = RED;
            
            if(bars[j].height < bars[minIndex].height){
                bars[minIndex].color = LIGHTGRAY;
                minIndex = j;
            }
            bars[j].color = LIGHTGRAY;
            j++;
        } else {
            int temp = bars[i].height;
            bars[i].height = bars[minIndex].height;
            bars[minIndex].height = temp;
            
            bars[minIndex].color = LIGHTGRAY;
            bars[i].color = LIGHTGRAY;
            
            i++;
            j = i + 1;
            minIndex = i;
        }
    } else {
        sorted = true;
        i = 0;
        j = 0;
        minIndex = 0;
    }
    DrawBars();
}

void InsertionSortStep(){
    static int i = 1;
    static int j = 0;
    static int key = 0;
    static bool placing = false;
    static bool sorted = false;
    
    if(lastAlgorithm != currentAlgorithm){
        i = 1;
        j = 0;
        key = 0;
        placing = false;
        sorted = false;
    }
    
    if(sorted){
        DrawBars();
        return;
    }
    
    if(i < NUM_BARS){
        if(!placing){
            key = bars[i].height;
            j = i - 1;
            placing = true;
        }
        
        if(j >= 0 && bars[j].height > key){
            bars[j + 1].height = bars[j].height;
            bars[j].color = RED;
            bars[j + 1].color = BLUE;
            
            bars[j].color = LIGHTGRAY;
            bars[j + 1].color = LIGHTGRAY;
            j--;
        } else {
            bars[j + 1].height = key;
            i++;
            placing = false;
        }
    } else {
        sorted = true;
        i = 1;
        j = 0;
        placing = false;
    }
    DrawBars();
}

void QuickSortStep(){
    static int stack[NUM_BARS];
    static int top = -1;
    static bool initialized = false;
    
    if(lastAlgorithm != currentAlgorithm){
        top = -1;
        initialized = false;
    }
    
    if(!initialized){
        stack[++top] = 0;
        stack[++top] = NUM_BARS - 1;
        initialized = true;
    }
    
    if(top >= 0){
        int end = stack[top--];
        int start = stack[top--];
        int pivot = bars[end].height;
        int i = start - 1;
        
        bars[end].color = BLUE;
        
        for(int j = start; j < end; j++){
            bars[j].color = RED;
            
            if(bars[j].height < pivot){
                i++;
                int temp = bars[i].height;
                bars[i].height = bars[j].height;
                bars[j].height = temp;
            }
            bars[j].color = LIGHTGRAY;
        }
        
        int temp = bars[i + 1].height;
        bars[i + 1].height = bars[end].height;
        bars[end].height = temp;
        
        bars[end].color = LIGHTGRAY;
        
        int pivotIndex = i + 1;
        
        if(pivotIndex - 1 > start){
            stack[++top] = start;
            stack[++top] = pivotIndex - 1;
        }
        if(pivotIndex + 1 < end){
            stack[++top] = pivotIndex + 1;
            stack[++top] = end;
        }
    }
    DrawBars();
}

void MergeSortStep(){
    static int tempArray[NUM_BARS];
    static int stack[NUM_BARS * 2];
    static int top = -1;
    static bool initialized = false;
    static int mergeLeft = 0, mergeRight = 0, mergeMid = 0;
    static int mergeIndex = 0, leftIndex = 0, rightIndex = 0;
    static bool merging = false;
    
    if(lastAlgorithm != currentAlgorithm){
        top = -1;
        initialized = false;
        merging = false;
    }
    
    if(!initialized){
        stack[++top] = 0;
        stack[++top] = NUM_BARS - 1;
        initialized = true;
    }
    
    if(!merging && top >= 0){
        mergeRight = stack[top--];
        mergeLeft = stack[top--];
        
        if(mergeLeft < mergeRight){
            mergeMid = mergeLeft + (mergeRight - mergeLeft) / 2;
            
            stack[++top] = mergeMid + 1;
            stack[++top] = mergeRight;
            
            stack[++top] = mergeLeft;
            stack[++top] = mergeMid;
            
            stack[++top] = mergeLeft;
            stack[++top] = mergeRight;
            stack[++top] = -1;
        }
    } else if(merging){
        if(leftIndex <= mergeMid && rightIndex <= mergeRight){
            bars[leftIndex].color = RED;
            bars[rightIndex].color = BLUE;
            
            if(bars[leftIndex].height <= bars[rightIndex].height){
                tempArray[mergeIndex++] = bars[leftIndex++].height;
            } else {
                tempArray[mergeIndex++] = bars[rightIndex++].height;
            }
        } else if(leftIndex <= mergeMid){
            bars[leftIndex].color = RED;
            tempArray[mergeIndex++] = bars[leftIndex++].height;
        } else if(rightIndex <= mergeRight){
            bars[rightIndex].color = BLUE;
            tempArray[mergeIndex++] = bars[rightIndex++].height;
        } else {
            for(int i = mergeLeft; i <= mergeRight; i++){
                bars[i].height = tempArray[i - mergeLeft];
                bars[i].color = LIGHTGRAY;
            }
            merging = false;
        }
    } else if(top >= 0 && stack[top] == -1){
        top--;
        mergeRight = stack[top--];
        mergeLeft = stack[top--];
        mergeMid = mergeLeft + (mergeRight - mergeLeft) / 2;
        
        mergeIndex = 0;
        leftIndex = mergeLeft;
        rightIndex = mergeMid + 1;
        merging = true;
    }
    
    DrawBars();
}

void HeapSortStep(){
    static int n = NUM_BARS;
    static int i = NUM_BARS / 2 - 1;
    static bool heapifying = true;
    static bool sorted = false;
    static int current = 0, largest = 0, left = 0, right = 0;
    static bool heapifyStep = false;
    
    if(lastAlgorithm != currentAlgorithm){
        n = NUM_BARS;
        i = NUM_BARS / 2 - 1;
        heapifying = true;
        sorted = false;
        heapifyStep = false;
    }
    
    if(sorted){
        DrawBars();
        return;
    }
    
    // Build max heap
    if(heapifying){
        if(!heapifyStep && i >= 0){
            current = i;
            heapifyStep = true;
        }
        
        if(heapifyStep){
            largest = current;
            left = 2 * current + 1;
            right = 2 * current + 2;
            
            bars[current].color = BLUE;
            if(left < n) bars[left].color = RED;
            if(right < n) bars[right].color = RED;
            
            if(left < n && bars[left].height > bars[largest].height){
                largest = left;
            }
            if(right < n && bars[right].height > bars[largest].height){
                largest = right;
            }
            
            if(largest != current){
                int temp = bars[current].height;
                bars[current].height = bars[largest].height;
                bars[largest].height = temp;
                current = largest;
            } else {
                bars[current].color = LIGHTGRAY;
                if(left < n) bars[left].color = LIGHTGRAY;
                if(right < n) bars[right].color = LIGHTGRAY;
                heapifyStep = false;
                i--;
            }
        }
        
        if(i < 0 && !heapifyStep){
            heapifying = false;
            i = NUM_BARS - 1;
        }
    } else {
        // Extract elements from heap one by one
        if(i > 0){
            int temp = bars[0].height;
            bars[0].height = bars[i].height;
            bars[i].height = temp;
            bars[i].color = GREEN;
            
            n--;
            current = 0;
            bool done = false;
            
            while(!done){
                largest = current;
                left = 2 * current + 1;
                right = 2 * current + 2;
                
                if(left < n && bars[left].height > bars[largest].height){
                    largest = left;
                }
                if(right < n && bars[right].height > bars[largest].height){
                    largest = right;
                }
                
                if(largest != current){
                    temp = bars[current].height;
                    bars[current].height = bars[largest].height;
                    bars[largest].height = temp;
                    current = largest;
                } else {
                    done = true;
                }
            }
            i--;
        } else {
            bars[0].color = GREEN;
            sorted = true;
        }
    }
    
    DrawBars();
}

void ShellSortStep(){
    static int gap = NUM_BARS / 2;
    static int i = 0;
    static int j = 0;
    static int temp = 0;
    static bool comparing = false;
    static bool sorted = false;
    
    if(lastAlgorithm != currentAlgorithm){
        gap = NUM_BARS / 2;
        i = 0;
        j = 0;
        temp = 0;
        comparing = false;
        sorted = false;
    }
    
    if(sorted){
        DrawBars();
        return;
    }
    
    if(gap > 0){
        if(!comparing && i < NUM_BARS){
            if(i < gap){
                i++;
            } else {
                temp = bars[i].height;
                j = i;
                comparing = true;
            }
        }
        
        if(comparing){
            bars[j].color = RED;
            if(j >= gap) bars[j - gap].color = BLUE;
            
            if(j >= gap && bars[j - gap].height > temp){
                bars[j].height = bars[j - gap].height;
                j -= gap;
            } else {
                bars[j].height = temp;
                bars[j].color = LIGHTGRAY;
                if(j >= gap) bars[j - gap].color = LIGHTGRAY;
                comparing = false;
                i++;
            }
        }
        
        if(i >= NUM_BARS){
            gap /= 2;
            i = gap;
        }
    } else {
        sorted = true;
    }
    
    DrawBars();
}

void CountingSortStep(){
    static int count[WINDOWS_HEIGHT];
    static int output[NUM_BARS];
    static int phase = 0; // 0: count, 1: cumulative, 2: place
    static int i = 0;
    static bool sorted = false;
    
    if(lastAlgorithm != currentAlgorithm){
        for(int k = 0; k < WINDOWS_HEIGHT; k++) count[k] = 0;
        phase = 0;
        i = 0;
        sorted = false;
    }
    
    if(sorted){
        DrawBars();
        return;
    }
    
    // Phase 0: Count occurrences
    if(phase == 0){
        if(i < NUM_BARS){
            bars[i].color = RED;
            count[bars[i].height]++;
            bars[i].color = LIGHTGRAY;
            i++;
        } else {
            phase = 1;
            i = 1;
        }
    }
    // Phase 1: Cumulative count
    else if(phase == 1){
        if(i < WINDOWS_HEIGHT){
            count[i] += count[i - 1];
            i++;
        } else {
            phase = 2;
            i = NUM_BARS - 1;
        }
    }
    // Phase 2: Place elements
    else if(phase == 2){
        if(i >= 0){
            bars[i].color = BLUE;
            output[count[bars[i].height] - 1] = bars[i].height;
            count[bars[i].height]--;
            bars[i].color = LIGHTGRAY;
            i--;
        } else {
            for(int k = 0; k < NUM_BARS; k++){
                bars[k].height = output[k];
                bars[k].color = LIGHTGRAY;
            }
            sorted = true;
        }
    }
    
    DrawBars();
}

void UpdateAlgorithm(){
    if(IsKeyPressed(KEY_ONE)){currentAlgorithm = BUBBLE; InitBars();}
    if(IsKeyPressed(KEY_TWO)){currentAlgorithm = SELECTION; InitBars();}
    if(IsKeyPressed(KEY_THREE)){currentAlgorithm = INSERTION; InitBars();}
    if(IsKeyPressed(KEY_FOUR)){currentAlgorithm = QUICK; InitBars();}
    if(IsKeyPressed(KEY_FIVE)){currentAlgorithm = MERGE; InitBars();}
    if(IsKeyPressed(KEY_SIX)){currentAlgorithm = HEAP; InitBars();}
    if(IsKeyPressed(KEY_SEVEN)){currentAlgorithm = SHELL; InitBars();}
    if(IsKeyPressed(KEY_EIGHT)){currentAlgorithm = COUNTING; InitBars();}
}

int main(){
    InitWindow(WINDOWS_WIDTH, WINDOWS_HEIGHT, "Sorting Algorithm Visualization!");
    SetTargetFPS(60);
    
    srand(time(NULL));
    InitBars();
    
    while(WindowShouldClose() == false){
        UpdateAlgorithm();
        
        BeginDrawing();
        ClearBackground(BLACK);
        
        if(currentAlgorithm == BUBBLE) BubbleSortStep();
        if(currentAlgorithm == SELECTION) SelectionSortStep();
        if(currentAlgorithm == INSERTION) InsertionSortStep();
        if(currentAlgorithm == QUICK) QuickSortStep();
        if(currentAlgorithm == MERGE) MergeSortStep();
        if(currentAlgorithm == HEAP) HeapSortStep();
        if(currentAlgorithm == SHELL) ShellSortStep();
        if(currentAlgorithm == COUNTING) CountingSortStep();
        
        DrawText(sortNames[currentAlgorithm], 10, 10, 20, WHITE);
        DrawText("Press 1-8 to change algorithm", 10, 35, 15, DARKGRAY);
        
        lastAlgorithm = currentAlgorithm;
        
        EndDrawing();
    }
    
    CloseWindow();
    return 0;
}