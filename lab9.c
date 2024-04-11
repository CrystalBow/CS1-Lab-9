#include <stdio.h>
#include <stdlib.h>

// RecordType
typedef struct RecordType
{
	int		id;
	char	name;
	int		order; 
}RecordType;

// Fill out this structure
typedef struct HashType
{
    RecordType* recordPtr;
    struct HashType* next;
}HashType;

// Compute the hash function
int hash(int x)
{
    return x%10;
}

// parses input file to an integer array
int parseData(char* inputFileName, struct RecordType** ppData)
{
	FILE* inFile = fopen(inputFileName, "r");
	int dataSz = 0;
	int i, n;
	char c;
	struct RecordType *pRecord;
	*ppData = NULL;

	if (inFile)
	{
		fscanf(inFile, "%d\n", &dataSz);
		*ppData = (struct RecordType*) malloc(sizeof(struct RecordType) * dataSz);
		// Implement parse data block
		if (*ppData == NULL)
		{
			printf("Cannot allocate memory\n");
			exit(-1);
		}
		for (i = 0; i < dataSz; ++i)
		{
			pRecord = *ppData + i;
			fscanf(inFile, "%d ", &n);
			pRecord->id = n;
			fscanf(inFile, "%c ", &c);
			pRecord->name = c;
			fscanf(inFile, "%d ", &n);
			pRecord->order = n;
		}

		fclose(inFile);
	}

	return dataSz;
}

// prints the records
void printRecords(struct RecordType pData[], int dataSz)
{
	int i;
	printf("\nRecords:\n");
	for (i = 0; i < dataSz; ++i)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType **elTable, int hashSz)
{
	int counter;
    HashType *temp, *prev;
    for (counter = 0; counter < hashSz; counter++) {
        printf("Hash Number: %d\n", counter);
        if (elTable[counter] == NULL) {
            printf("\tContains Nothing\n\n");
            continue;
        }
        temp = elTable[counter];
        prev = temp;
        temp = prev->next;
        while (prev != NULL) {
            printf("\t%d %c %d\n", prev->recordPtr->id,prev->recordPtr->name, prev->recordPtr->order);
            free(prev);
            prev = temp;
            if (temp != NULL) {
                temp = temp->next;
            }
        }
    }
    free(elTable);
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0, counter, hashNum;

	recordSz = parseData("input_lab_9.txt", &pRecords);
	printRecords(pRecords, recordSz);
	// Your hash implementation
    HashType** elTable = (HashType**) calloc(10, sizeof(HashType*));
    HashType *temp, *prev;
    for (counter = 0; counter < recordSz; counter++) {
        hashNum = hash(pRecords[counter].id);
        temp = elTable[hashNum];
        prev = temp;
        if (temp == NULL) {
            elTable[hashNum] = (HashType*) calloc(1, sizeof(HashType));
            elTable[hashNum]->recordPtr = &pRecords[counter];
            continue;
        }
        while (temp != NULL) {
            prev = temp;
            temp = temp->next;
        }
        temp = (HashType*) calloc(1, sizeof(HashType));
        temp->recordPtr = &pRecords[counter];
        prev->next = temp;
    }
    displayRecordsInHash(elTable, 10);
    free(pRecords);
    return 0;
}