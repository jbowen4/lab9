#include <stdio.h>
#include <stdlib.h>

#define TABLE_SIZE 31

// RecordType
struct RecordType
{
	int		id;
	char	name;
	int		order; 
};

struct Node {
	struct RecordType record;
	struct Node *next;
};

// Fill out this structure
struct HashType
{
	struct Node **buckets;
};

// Compute the hash function
int hash(int x)
{
	while (x >= 100) {
    x /= 10;
  }
	return x % 31;
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
	for (i = 0; i < dataSz; i++)
	{
		printf("\t%d %c %d\n", pData[i].id, pData[i].name, pData[i].order);
	}
	printf("\n\n");
}

// display records in the hash structure
// skip the indices which are free
// the output will be in the format:
// index x -> id, name, order -> id, name, order ....
void displayRecordsInHash(struct HashType *pHashArray, int hashSz)
{
	int i;

	for (i=0;i<hashSz;++i)
	{
		struct Node *tmp = pHashArray->buckets[i];
		if (tmp == NULL) continue;
		printf("index %d", i);
		while (tmp != NULL) {
			printf(" -> ");
			printf("%d, %c, %d", tmp->record.id, tmp->record.name, tmp->record.order);
			tmp = tmp->next;
		}
		printf("\n");
	}
}

int main(void)
{
	struct RecordType *pRecords;
	int recordSz = 0;

	recordSz = parseData("input.txt", &pRecords);
	printRecords(pRecords, recordSz);
	
	struct HashType *pHashArray = malloc(sizeof(struct HashType));
	pHashArray->buckets = malloc(sizeof(struct Node*) * TABLE_SIZE);

	for (int i = 0; i < recordSz; i++) {
		int index = hash(pRecords[i].id);

		struct Node *newNodePtr = malloc(sizeof(struct Node));
		newNodePtr->record = pRecords[i];
		newNodePtr->next = NULL;

		if (pHashArray->buckets[index] == NULL) {
			pHashArray->buckets[index] = newNodePtr;
		} else {
			struct Node *tmp = pHashArray->buckets[index];
			while (tmp->next != NULL) {
				tmp = tmp->next;
			}
			tmp->next = newNodePtr;
		}
	}

	displayRecordsInHash(pHashArray, TABLE_SIZE);


	for (int i = 0; i < recordSz; i++) {
		struct Node *tmp;
		struct Node *curr = pHashArray->buckets[i];
		while (curr) {
			tmp = curr;
			curr = curr->next;
			free(tmp);
		}
	}

	free(pHashArray->buckets);
	free(pHashArray);
	free(pRecords);
}