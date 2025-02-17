#include "autocomplete.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// Comparator function for read_in_terms qsort
int compare_terms(const void *a, const void *b) {
    // Cast the void pointers to 'term' pointers
    const struct term *termA = (const struct term *)a;
    const struct term *termB = (const struct term *)b;

    // Compare the 'term' strings lexicographically using strcmp
    return strcmp(termA->term, termB->term);
}

void read_in_terms(struct term **terms, int *pnterms, char *filename) {
    int i = 0;
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        printf("Error opening file\n");
        return;
    }

    char line[200];
    *pnterms = 0;

    // Skip the first line
    if (!fgets(line, sizeof(line), fp)) {
        printf("Error reading file\n");
        fclose(fp);
        return;
    }

    // Count the number of lines (terms) in the file, starting from the second line
    while (fgets(line, sizeof(line), fp)) {
        (*pnterms)++;
    }

    // Allocate memory for the terms
    *terms = (struct term*)malloc(*pnterms * sizeof(struct term));
    if (*terms == NULL) {
        printf("Memory allocation failed\n");
        fclose(fp);
        return;
    }

    rewind(fp);  // Rewind to the beginning of the file

    // Skip the first line again
    if (!fgets(line, sizeof(line), fp)) {
        printf("Error reading file\n");
        fclose(fp);
        return;
    }

    // Read each line and extract the weight and term
    for (i = 0; i < *pnterms; i++) {
        if (fgets(line, sizeof(line), fp)) {
            char *weight_char = strtok(line, "\t");  
            char *term_to_save = strtok(NULL, "\n"); 

            if (weight_char != NULL && term_to_save != NULL) {
                (*terms)[i].weight = atof(weight_char);  
                strcpy((*terms)[i].term, term_to_save);
            }
        }
    }

    fclose(fp);

    // Sort the terms lexicographically
    qsort(*terms, *pnterms, sizeof(struct term), compare_terms);
}

int lowest_match(struct term* terms, int nterms, char* substr) {
    if (nterms == 0) {
        return -1;
    }
    int left = 0, right = nterms - 1, res = -1;
    int len_substr = strlen(substr);

    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strncmp(terms[mid].term, substr, len_substr);
        if (cmp < 0) {
            left = mid + 1;
        }
        else {
            res = mid;
            right = mid - 1;
        }
    }
    return (res != -1 && strncmp(terms[res].term, substr, len_substr) == 0) ? res : -1;
}

int highest_match(struct term* terms, int nterms, char* substr) {
    int left = 0, right = nterms - 1, res = -1;
    int len_substr = strlen(substr);
    while (left <= right) {
        int mid = (left + right) / 2;
        int cmp = strncmp(terms[mid].term, substr, len_substr);
        if (cmp < 0) {
            left = mid + 1;
        }
        else if (cmp == 0) {
                res = mid;
                left = mid + 1;
        }
        else {
            right = mid - 1;
        }

    }
    
    return (res != -1 && strncmp(terms[res].term, substr, len_substr) == 0) ? res : -1;
}

// Comparator function for autocomplete qsort
int compare_by_weight(const void *a, const void *b) {
    double weight_a = ((struct term*)a)->weight;
    double weight_b = ((struct term*)b)->weight;

    // To sort in descending order, reverse the comparison:
    return (weight_b - weight_a);
}

void autocomplete(struct term** answer, int* n_answer, struct term* terms, int nterms, char* substr) {
    int start = lowest_match(terms, nterms, substr);
    if (start == -1) {
        *answer = NULL;
        *n_answer = 0;
        return;
    }
    int i = start, count = 0;
    while (i < nterms && strncmp(terms[i].term, substr, strlen(substr)) == 0) {
        count++;
        i++;
    }
    *answer = (struct term*)malloc(count * sizeof(struct term));
    *n_answer = count;
    for (i = 0; i < count; i++) {
        (*answer)[i] = terms[start + i];
    }
    qsort(*answer, *n_answer, sizeof(struct term), compare_by_weight);
}