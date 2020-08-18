#include <bits/stdc++.h>
using namespace std;
#define ALPHABET_SIZE 26
#define false 0
#define true 1
#define MAX_WORD_SIZE   50
#define MAX_MEANING_SIZE 1000
int countword = 0;

int letterToInt(char letter)
{
    if (letter >= 'A' && letter <= 'Z')
    {
        return letter - 'A';
    }

    else if (letter >= 'a' && letter <= 'z')
    {
        return letter - 'a';
    }

    return -1;
}

class trieNode
{
    public:
    trieNode* children[ALPHABET_SIZE];
    bool isEndOfWord;
    string value; //meaning
    trieNode()
    {
        this->isEndOfWord=false;
        for(int i=0;i< ALPHABET_SIZE;i++)
            this->children[i]=NULL;
    }
};
int insert(trieNode *root, string word, string meaning)
{
    trieNode *node = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letterToInt(word[i]);

        if (!node->children[index])
            node->children[index] = new trieNode();

        node = node->children[index];
    }

    node->isEndOfWord = true;
    node->value = meaning;
    countword++;

    return true;
}

int readWordsFromFile(trieNode *root, const char* filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        cout<<"could not find/open file \"%s\"\n"<<filename;
        return false;
    }

    char word[MAX_WORD_SIZE];
    char meaning[MAX_MEANING_SIZE];



    while (fscanf(file, "%s %[^\n]", word, meaning) > 1)
    {
        if (!insert(root, word, meaning))
        {
            fclose(file);
            return false;
        }
    }

    fclose(file);


    return true;
}

bool search(trieNode *root, string word)
{
    trieNode *node= root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letterToInt(word[i]);

        if (!node->children[index])
            return false;

        node = node->children[index];
    }

    if (node!= NULL && node->isEndOfWord)  {
        cout << "The meaning of the word is: \n";
        cout << node->value;
    }

    return true;
}

bool deleteWord(trieNode *root, string word)
{
    trieNode *node= root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letterToInt(word[i]);

        if (!node->children[index])
            return false;

        node= node->children[index];
    }

    if (node!= NULL && node->isEndOfWord)  {
        node->isEndOfWord = false;
        return true;
    }
}

void alphabeticalOrder(trieNode* root, char allWords[], int index)
{
    trieNode* node= root;

    if(node != NULL && node->isEndOfWord == true) {
        for(int j = 0; j < index; j++)  {
            cout << allWords[j];
            }

        cout << ": ";
        cout << node->value << "\n";
    }

    for(int i = 0; i < ALPHABET_SIZE; i++)  {
        if(node->children[i] != NULL) {
            allWords[index] = 'a' + i;
            alphabeticalOrder(node->children[i], allWords, index + 1);
        }
    }
}

void printPrefixSearch(trieNode* root, char allWords[], int index, string prefix)
{
    trieNode* node = root;

    if(node != NULL && node->isEndOfWord == true) {
        cout << prefix;

        for(int j = 0; j < index; j++)  {
            cout << allWords[j];
            }

        cout << ": ";
        cout << node->value << "\n";
    }

    for(int i = 0; i < ALPHABET_SIZE; i++)  {
        if(node->children[i] != NULL) {
            allWords[index] = 'a' + i;
            printPrefixSearch(node->children[i], allWords, index + 1, prefix);
        }
    }
}

void prefixSearch(trieNode* root, string prefix)
{
    trieNode* node = root;

    for(int i = 0; i < prefix.length(); i++)    {
        int index = letterToInt(prefix[i]);
        node = node->children[index];
    }

    char allWords[50];
    printPrefixSearch(node, allWords, 0, prefix);
}
void printsug(trieNode* root, char allWords[], int index, string prefix)
{
    trieNode* node = root;

    if(node != NULL && node->isEndOfWord == true) {

        cout <<"\t\t"<< prefix;

        for(int j = 0; j < index; j++)  {
            cout << allWords[j];
            }
            cout<<endl;
    }

    for(int i = 0; i < ALPHABET_SIZE; i++)  {
        if(node->children[i] != NULL) {
            allWords[index] = 'a' + i;
            printsug(node->children[i], allWords, index + 1, prefix);
        }
    }
}
void printsugest(trieNode* root, string prefix)
{
    trieNode* node = root;

    for(int i = 0; i < prefix.length(); i++)    {
        int index = letterToInt(prefix[i]);
        node = node->children[index];
    }

    char allWords[50];
    printsug(node, allWords, 0, prefix);
}

bool searchSuggestedWord(trieNode* root, string word)
{
     trieNode *node = root;

    for (int i = 0; i < word.length(); i++)
    {
        int index = letterToInt(word[i]);

        if (!node->children[index])
            return false;

        node = node->children[index];
    }

    return true;
}

bool suggestedWords(trieNode* root, string word)
{
    int count = 0;

    for(int i = 0; i < word.length(); i++)
    {
        string temp = word;

        for(int j = 0; j < ALPHABET_SIZE; j++)
        {
                if(j != letterToInt(word[word.length() - i - 1]))
            {
                temp[word.length() - i - 1] = j + 97;
                if(searchSuggestedWord(root, temp))
                 {
                        if(count == 0)
                         {
                            cout << "\t\tSuggested words are: "<<endl;
                        }
                        count++;

                    printsugest(root, temp);
                    count++;
                }
            }
        }

    }
    if(searchSuggestedWord(root, word)) {
                        if(count == 0)  {
            cout << "\t\tSuggested words are: "<<endl;
        }
        count++;

        printsugest(root, word);
                count++;
                }

        if(count == 0)
        {
            cout << "\nSorry we couldn't find any words related to " << word << " in our Dictionary.\n";
            return false;
        }
        else
        {
            return true;
        }
}
void menu(){
    trieNode *root = new trieNode();

    readWordsFromFile(root, "words.txt");
     string command;
    do  {
    system("cls");
    cout<<"\n\n\t\t\t\t|=========================================================|"<<endl;
    cout<<"\t\t\t\t|                                                         |"<<endl;
    cout<<"\t\t\t\t|                       DICTIONARY                        |"<<endl;
    cout<<"\t\t\t\t|                                                         |"<<endl;
    cout<<"\t\t\t\t|=========================================================|"<<endl;
            cout << "\t\t\t    Welcome to our Dictionary. Our Dictionary right now contains " << countword << " words."<<endl;
        cout << "    \nWhat would you like to do?"<<endl;
        cout << "    1. Insert a word."<<endl;
        cout << "    2. Search a word."<<endl;
        cout << "    3. Delete a word."<<endl;
        cout << "    4. Print dictionary in alphabetical order."<<endl;
        cout << "    5. Prefix search."<<endl;
        cout << "    6. Show suggested words."<<endl;;
        cout << endl;
        fflush(stdin);

        int call;
        cout<<"\t\t Enter your choice:- ";
        cin >> call;
        cout <<endl;

        string word;
        string meaning;

        switch(call)    {

            case 1: cout << "    Enter the word you would like to insert: ";
                    cin >> word;
                    if(!search(root,word))
                    {
                    cout << "    Enter its meaning: ";
                    cin.ignore();
                    getline(cin,meaning);

                    if(insert(root, word, meaning)) {
                        cout << word << "     has been added to the Dictionary."<<endl;
                    }
                    }
                    else{
                        cout<<"\n\t\t!!!Word already Exits with meaning provided above.!!!";
                    }
                    break;

            case 2: cout << "    Enter the word you would like to search: ";
                    cin >> word;

                    if(!search(root, word))  {
                        cout << "    Sorry, the word you searched for doesn't exist. Would you like to add it to the Dictionary.(Yes/No) ";

                        string add;
                        cin >> add;

                        if(add == "yes" || add == "Yes" || command == "YES")  {
                            cout << "    Enter its meaning: ";
                            cin >> meaning;

                            if(insert(root, word, meaning)) {
                            cout << word << "     has been added to the Dictionary."<<endl;
                            }
                        }
                    }

                    break;

            case 3: cout << "    Which word would you like to delete? ";
                    cin >> word;

                    if(deleteWord(root, word))  {
                        cout << word << "     has been successfully deleted from the Dictionary."<<endl;
                    }

                    else    {
                        cout << "    No such word exists in the Dictionary."<<endl;
                    }

                    break;

            case 4: char allWords[50];
                    alphabeticalOrder(root, allWords, 0);
                    cout << "\n";

                    break;

            case 5: cout << "    Enter the word you would like to use as a prefix: ";
                    cin >> word;
                    cout << "\n";

                    prefixSearch(root, word);

                    break;

            case 6: cout << "    Enter the word for whose suggested words you want to see: ";
                    cin >> word;

                    suggestedWords(root, word);

                    break;

            default: cout << "    Enter a valid entry.";
        }
    fflush(stdin);
        cout << "    \n\nWould you like to continue or exit?(Yes/N0) ";
        cin >> command;
    }
    while(command == "yes" || command == "Yes" || command == "YES");
}
void delay(int j)
{   int i;
    for(i=0;i<j;i++)
    {

	}

}
int main()  {
    system("cls");
    cout << "\n\t\t\t\t\t\tWelcome to our Dictionary"<<endl;
    cout<<"\n\n\t\t\tPress any key to open:-  ";
    getchar();

   system("cls");
        cout<<"\n\n\n\n\n\t\t\t\t\t";
        char a=177,b=178;
        cout<<"Loading.....";
        cout<<endl<<endl;
        cout<<"\t\t\t\t\t";
        for(int i=0;i<30;i++)
        {
            cout<<a;
        }
        cout<<"\r";
        cout<<"\t\t\t\t\t";
        for(int i=0;i<30;i++)
        {
            delay(21000000);
            cout<<b;

        }

          system("cls");
          menu();



    cout << "    Thanks for using our Dictionary."<<endl;
    return 0;
}
