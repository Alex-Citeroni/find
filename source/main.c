#include "find.h"

void help()
{
  printf("\nHelp\n\nPer generare un nuovo report scrivere:\n\n\t./find (--words|-w) <wordfile> (--input|-i) <inputfile>\n\nPer salvare il report su un file bisognerà aggiungere il parametro:\n\n\t--output|-o <outputfile>\n\nDurante l'analisi è possibile ignorare i file con specifiche estensioni, aggiungendo:\n\n\t--exclude|-e <ext>\n\nPer vedere il processo di analisi si può aggiungere:\n\n\t --verbose|-v \n\nCreato il file di report, find può recuperare le informazioni salvate\n\nStampare la lista dei file dove c'è almeno <n> volte la parola <word>:\n\n\t./find --report|-r <reportfile> --show <word> <n>\n\nStampare tutte le posizioni dove la parola <word> è nel file <file>:\n\n\t./find --report|-r <reportfile> --show <word> --file <file>\n");
}

int main(int argc, char **argv)
{
  parameter parameter = {"NOT_SET", "NOT_SET", "NOT_SET", "NOT_SET", "",
                         "NOT_SET", "NOT_SET", 1, 0},
            *parameters = &parameter;
  struct option longOptions[] = {{"verbose", no_argument, 0, 'v'},
                                 {"word", required_argument, 0, 'w'},
                                 {"input", required_argument, 0, 'i'},
                                 {"output", required_argument, 0, 'o'},
                                 {"report", required_argument, 0, 'r'},
                                 {"exclude", required_argument, 0, 'e'},
                                 {"show", required_argument, 0, SHOW},
                                 {"file", required_argument, 0, INNER_FILE},
                                 {0, 0, 0, 0}};
  int c, optionIndex, invalidArgs = 0;
  while ((c = getopt_long(argc, argv, "vw:i:o:r:e:", longOptions, &optionIndex)) != -1)
  {
    switch (c)
    {
    case 'w':
      strcpy(parameters->word, optarg);
      break;

    case 'i':
      strcpy(parameters->input, optarg);
      break;

    case 'o':
      strcpy(parameters->output, optarg);
      break;

    case 'e':
      strcpy(parameters->toExclude, optarg);
      break;

    case 'r':
      strcpy(parameters->report, optarg);
      break;

    case SHOW:
      strcpy(parameters->wordToShow, optarg);
      for (; optind < argc && *argv[optind] != '-'; optind++)
        parameters->wordOccurrences = atoi(argv[optind]);
      break;

    case INNER_FILE:
      strcpy(parameters->innerFile, optarg);
      break;

    case 'v':
      parameters->isVerbose = 1;
      break;

    default:
      invalidArgs = 1;
      help();
      break;
    }
  }
  int reportDirectory = isDirectory(parameters->report),
      wordDirectory = isDirectory(parameters->word),
      inputDirectory = isDirectory(parameters->input),
      word = strcmp(parameters->word, "NOT_SET"),
      input = strcmp(parameters->input, "NOT_SET"),
      report = strcmp(parameters->report, "NOT_SET"),
      wordShow = strcmp(parameters->wordToShow, "NOT_SET"),
      innerFile = strcmp(parameters->innerFile, "NOT_SET");
  if (word != 0 && input != 0 && !invalidArgs)
    if (wordDirectory == -2 && inputDirectory == -2)
      findWord(parameters->input, parameters->word, parameters->toExclude, parameters->output, parameters->isVerbose ? 1 : 0);
    else
    {
      if (wordDirectory != -2)
        printf("\nInserisci un percorso valido per il file delle parole!\n");
      if (inputDirectory != -2)
        printf("\nInserisci un percorso valido per il file del percorso!\n");
    }
  else if (report != 0 && wordShow != 0 && (innerFile == 0 || innerFile != 0) && !invalidArgs)
    if (reportDirectory == -2 && innerFile == 0)
      findPathFromReport(parameters->wordToShow, parameters->report, parameters->wordOccurrences);
    else if (reportDirectory == -2 && innerFile != 0)
      findOccurrencesFromFile(parameters->wordToShow, parameters->report, parameters->innerFile);
    else
      printf("\nInserisci un percorso valido per il report!\n");
  else
  {
    puts("Argomento invalido\n");
    help();
  }
  return 0;
}