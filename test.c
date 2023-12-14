#include "rand_malloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int line_length;
int number_of_lines;

char *getLine(char *line, char **formatted_text)
{
	line_length = 0;
	int capacity = 0;
	char character = getchar();

	while (character != '\n' && character != EOF)
	{
		if (line_length == capacity)
		{
			if (capacity != 0)
			{
				capacity *= 2;
			}
			else
			{
				capacity = 1;
			}

			char *new_line = (char *)realloc(line, capacity * sizeof(char));

			if (new_line == NULL)
			{
				printf("Bad allocation, null pointer returned\n");
				free(line);
				for (int line = 0; line < number_of_lines; line++)
				{
					free(formatted_text[line]);
				}
				free(formatted_text);
				exit(EXIT_FAILURE);
			}
			line = new_line;
		}

		line[line_length] = character;
		line_length++;
		character = getchar();
	}

	if (character == EOF && line_length == 0)
	{
		free(line);
		return NULL;
	}

	char *new_line = (char *)realloc(line, (line_length + 2) * sizeof(char));

	if (new_line == NULL)
	{
		printf("Bad allocation, null pointer returned\n");
		free(line);
		for (int line = 0; line < number_of_lines; line++)
		{
			free(formatted_text[line]);
		}
		free(formatted_text);
		exit(EXIT_FAILURE);
	}

	line = new_line;
	line_length++;
	line[line_length - 1] = '\n';
	line[line_length] = '\0';

	return line;
}

void reverse_words(char *not_formatted_line)
{
	char *word_start = not_formatted_line;
	char *word_end = not_formatted_line;
	char temp;

	while (*word_end)
	{
		word_end++;
		if (*word_end == ' ' || *word_end == '\n' || *word_end == '\0')
		{
			char *word_end_reversed = word_end - 1;
			while (word_start < word_end_reversed)
			{
				temp = *word_start;
				*word_start = *word_end_reversed;
				*word_end_reversed = temp;

				word_start++;
				word_end_reversed--;
			}
			word_start = word_end + 1;
		}
	}

	char *line_start = not_formatted_line;
	char *line_end = word_end - 1;
	while (line_start < line_end)
	{
		temp = *line_start;
		*line_start = *line_end;
		*line_end = temp;

		line_start++;
		line_end--;
	}
}

void reverse_lines(char **not_formatted_text)
{
	char *temp_line;

	for (int line = 0; line < number_of_lines / 2; line++)
	{
		temp_line = not_formatted_text[line];
		not_formatted_text[line] = not_formatted_text[number_of_lines - 1 - line];
		not_formatted_text[number_of_lines - 1 - line] = temp_line;
	}

	for (int line = 0; line < number_of_lines; line++)
	{
		reverse_words(not_formatted_text[line]);
	}
}

void process_text(char **formatted_text)
{
	printf("\n");
	for (int line = 0; line < number_of_lines; line++)
	{
		printf("%s", formatted_text[line]);
		free(formatted_text[line]);
	}
	printf("\n");
	free(formatted_text);
}

void get_whole_text(char *single_line)
{
	char **text = NULL;

	number_of_lines = 0;
	int capacity = 0;
	char *whole_line = getLine(single_line, text);

	while (whole_line != NULL)
	{
		if (number_of_lines == capacity)
		{
			if (capacity != 0)
			{
				capacity *= 2;
			}
			else
			{
				capacity = 1;
			}

			char **new_text = (char **)realloc(text, capacity * sizeof(char *));

			if (new_text == NULL)
			{
				printf("Bad allocation, null pointer returned\n");
				for (int line = 0; line < number_of_lines; line++)
				{
					free(text[line]);
				}
				free(text);
				free(whole_line);
				exit(EXIT_FAILURE);
			}
			text = new_text;
		}

		text[number_of_lines] = whole_line;
		number_of_lines++;
		whole_line = getLine(single_line, text);
	}

	reverse_lines(text);
	process_text(text);

	free(single_line);
	free(whole_line);
}

int main()
{
	char *line = NULL;

	printf("Enter text, which has to be formatted:\n");
	printf("-------------------------------------\n");
	get_whole_text(line);
}