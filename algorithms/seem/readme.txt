This is Cody Skala (cws26) and here is my Algorithms Project 3(Seem Carving).
Everything is included that needs to be and has been tested on a lab machine with Linux.
If anything is missing, please let me know so I may correct it accordingly.
----------------------------------------------------------------------------------------
HOW TO TEST
----------------------------------------------------------------------------------------
This program has been tested on the lab machines using Linux. To run the program, type:
  make

To actually run the program, now type:
  java SeemCarving filename x y

One of the provided files is the excelTextCase.pgm.
This can be compared against the xlsx file to verify all is working correctly.
For example, if the excel file shows (1, 0) this means remove 1 vertical and
zero horizontal seems. This can be tested by running:
  java SeemCarving excelTestCase.pgm 1 0

This will produce a file called excelTestCase_processed.pgm which when opened in
a test editor can be viewed as a regular pgm file.

All the results I received seemed to match except the last case (1,5).
This is because the spreadsheet is not entirely correct and only shows the result
for (1, 4) instead of the expected (1, 5).

The program should be able to support the following file formats:
  pbm, pgm, ppm, png, jpg, gib, bmp

Also, there is a screen shot of removing 100 vertical and 100 horizontal seems on
twoBalls.pgm. I am unsure what the screen shot was supposed to be so this is provided.

Finally, if more than 100 seems are removed, the program may take longer to execute.
All cases tested should be able to run in under 10 seconds.
----------------------------------------------------------------------------------------
BONUS
----------------------------------------------------------------------------------------
This program also supports colored images
----------------------------------------------------------------------------------------
Overall, this was a fun project and learned a lot about seem-carving/dynamic-programming
and look forward to working with more algorithm projects as the semester goes along.
