#include "pdf_form_fill.h"

int main(int argc, char** argv) {
  EStatusCode status = eSuccess;
  PDFWriter writer;
  pdf_form_fill pff;

  do {
    status = writer.ModifyPDF(
      argv[1],
      ePDFVersion13,
      argv[2],
      LogConfiguration(true, true, argv[2])
    );

    if(status != eSuccess) {
      printf("failed to start PDF\n");
      break;
    }

    pff.fillForm(writer, {
      {"Given Name Text Box"       , "Eric"},
      {"Family Name Text Box"      , "Jones"},
      {"House nr Text Box"         , "someplace"},
      {"Address 1 Text Box"        , "somewhere 1"},
      {"Address 2 Text Box"        , "somewhere 2"},
      {"City Text Box"             , "somehwere 3"},
      {"Postcode Text Box"         , "123456"},
      {"Country Combo Box"         , "Spain"},
      {"Height Formatted Field"    , "198"},
      {"Driving License Check Box" , true},
      {"Favourite Colour List Box" , "Brown"},
      {"Language 1 Check Box"      , true},
      {"Language 2 Check Box"      , true},
      {"Language 3 Check Box"      , false},
      {"Language 4 Check Box"      , false},
      {"Language 5 Check Box"      , true},
      {"Gender List Box"           , "Man"},
      }
    );

    status = writer.EndPDF();
    if(status != eSuccess) {
      break;
    }
  } while(false);

  return 0;
}
