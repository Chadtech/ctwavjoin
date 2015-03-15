#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "./wavWrite.h"

int main(int numberOfArgs, char ** args) {


  const char * outputFileName = args[1];

  int numberOfFilesToOpen = numberOfArgs - 2;
      numberOfFilesToOpen /= 2;

  long audioPlacements [ numberOfFilesToOpen ];
  const char * audioFileNames [ numberOfFilesToOpen ];



  int argumentIndex = 2;
  int index = 0;
  while (argumentIndex < numberOfArgs) {

    if ((argumentIndex % 2) == 0 ){
      audioFileNames[ index ] = args[ argumentIndex ];
    }
    else {
      audioPlacements[ index ] = 0;
      audioPlacements[ index ] += atol(args[ argumentIndex ]);
      index++;
    }

    argumentIndex++;
  }



  long lastPlacement = 0;
  int  lastPlacementIndex = 0;
  argumentIndex = 0;
  while (argumentIndex < numberOfFilesToOpen) {

    if (audioPlacements[ argumentIndex ] > lastPlacement) {
      lastPlacement      = audioPlacements[ argumentIndex ];
      lastPlacementIndex = argumentIndex;
    } 

    argumentIndex++;
  }



  std::ifstream lastFile;  
  lastFile.open( audioFileNames[ lastPlacementIndex ], std::ifstream::in);



  lastFile.seekg(40);
  long lastDuration = 0;

       lastDuration += long(lastFile.get());
       lastDuration += long(lastFile.get()) * 256;
       lastDuration += long(lastFile.get()) * 65536;
       lastDuration += long(lastFile.get()) * 16777216;
       lastDuration /= 4;

  int durationOfOutput = lastDuration + audioPlacements[ lastPlacementIndex ];



  short * outputDataLeft;
  short * outputDataRight;
  outputDataLeft  = new short [ durationOfOutput ];
  outputDataRight = new short [ durationOfOutput ];
  index = 0;
  while (index < durationOfOutput) {
    outputDataLeft[index] = 0;
    outputDataRight[index] = 0;
    index++;
  }



  long indexl = 0;
  while (indexl < lastDuration) {

    short thisSampleLeft = short(lastFile.get());
          thisSampleLeft += short(lastFile.get()) * 256;

    outputDataLeft[ indexl + lastPlacement] = thisSampleLeft;

    short thisSampleRight = short(lastFile.get());
          thisSampleRight = short(lastFile.get()) * 256;

    outputDataRight[ indexl + lastPlacement ] = thisSampleRight;

    indexl++;
  }




  index = 0;
  while (index < (numberOfFilesToOpen )) {

    if (index != lastPlacementIndex ){

   
      std::ifstream thisFile;
      thisFile.open(audioFileNames[ index ], std::ifstream::in);

      thisFile.seekg(40);
      long thisDuration = 0;

           thisDuration += long(thisFile.get());
           thisDuration += long(thisFile.get()) * 256;
           thisDuration += long(thisFile.get()) * 65536;
           thisDuration += long(thisFile.get()) * 16777216;
           thisDuration /= 4;



      indexl = 0;
      while (indexl < thisDuration) {

        short thisSampleLeft = short(thisFile.get());
              thisSampleLeft += short(thisFile.get()) * 256;


        outputDataLeft[ indexl + audioPlacements[ index ] ] = thisSampleLeft;

        short thisSampleRight = short(thisFile.get());
              thisSampleRight += short(thisFile.get()) * 256;


        outputDataRight[ indexl + audioPlacements[ index ] ] = thisSampleRight;

        indexl++;
      }
    }


    index++;

  }

  short * mixedOutput;
  mixedOutput = new short [ durationOfOutput * 2];

  index = 0;
  while (index < durationOfOutput) {

    mixedOutput[ index * 2 ] = outputDataLeft[ index ];
    mixedOutput[ (index * 2) + 1 ] = outputDataRight[ index ];

    index++;
  }

  writeWAVData( outputFileName, mixedOutput, durationOfOutput * 4, 44100, 2);

}