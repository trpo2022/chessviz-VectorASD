#include <stdio.h>
#include <stdlib.h>

typedef const char* text;

int main(int argc, text* args)
{
    printf("Аргументы:\n");
    for (int i = 0; i < argc; i++) {
        printf("%u %s\n", i, args[i]);
    }
    FILE* file;
    if ((file = fopen("result.html", "w")) == NULL) {
        printf("Не удалось открыть файл на запись\n");
        return 0;
    }
    fprintf(file, "<html><head><title>Шахматная доска</title><body>");
    
    fprintf(file, "<table>");
    for(int Y = 0; Y < 8; Y++) {
    	fprintf(file, "<tr>");
    	for(int X = 0; X < 8; X++) {
    	    fprintf(file, "<td>X</td>");   		
    	}
    	fprintf(file, "</tr>");
    }
    fprintf(file, "</table>");
    
    fprintf(file, "После каждого хода обновляйте страницу");
    fprintf(file, "</body></html>");
    fclose(file);
    printf("Создание шахматной доски выполнено удачно\n");
    return 0;
}
