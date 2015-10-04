#include "THnSparse.h"

void mergeScript()
{
    // load libraries
    
    gROOT->ProcessLine(".! ls *.root > files.txt"); 
    TFileMerger merger;
    FILE *f;
    f = fopen("files.txt", "r");
    if ( f == 0 )
        cout << "no such a file!" << endl;
    char strFileName[120];
    char strFileNameCopy[120];
    while ( f != EOF )
    {
        fscanf(f, "%s", strFileName );
        if ( strstr( strFileNameCopy, strFileName ) )
        {
            cout << "end of file reached! (reading the same line)" << endl;
            break;
        }
        strcpy( strFileNameCopy, strFileName ); //remember name
        cout << strFileName << endl;
        merger.AddFile( strFileName );
    }
    cout << "Start merging..." << endl;
    merger.OutputFile("MergedOutput.root");
    merger.Merge();
    cout << "Merging finished." << endl;
}