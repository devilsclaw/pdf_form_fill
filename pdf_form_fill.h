#ifndef __PDF_FORM_FILL_H__
#define __PDF_FORM_FILL_H__

#include <stdint.h>
#include <stdexcept>
#include <vector>
#include <string>
#include <algorithm>

#include "PDFParser.h"
#include "PDFObjectCast.h"
#include "PDFDictionary.h"
#include "PDFDocumentCopyingContext.h"
#include "InputFile.h"
#include "RefCountPtr.h"
#include "PDFArray.h"
#include "PDFName.h"
#include "PDFIndirectObjectReference.h"
#include "PDFStreamInput.h"
#include "IByteReader.h"
#include "EStatusCode.h"
#include "ParsedPrimitiveHelper.h"
#include "PDFHexString.h"
#include "UnicodeString.h"
#include "AbstractContentContext.h"
#include "PDFWriter.h"
#include "PDFModifiedPage.h"
#include "ObjectsContext.h"
#include "DictionaryContext.h"
#include "PDFPage.h"
#include "PDFRectangle.h"
#include "PageContentContext.h"
#include "PDFFormXObject.h"
#include "XObjectContentContext.h"
#include "PDFUsedFont.h"
#include "PDFInteger.h"
#include "PDFReal.h"
#include "PDFTextString.h"
#include "PDFLiteralString.h"
#include "Trace.h"

class pdf_form_fill {
  public:
    const std::string _NULL_ = "__MAGIC_NULL__";

    class pdf_value_t {
      public:
        enum {
          NONE,
          INTEGER,
          DOUBLE,
          BOOL,
          STRING,
          PDFARRAY,
        };
      private:
        int p_type = NONE;
        union {
          long long i_value;
          double    d_value;
          bool      b_value;
        };
        std::string s_value;
        PDFObjectCastPtr<PDFArray> a_value;

      public:
        void set() {
          p_type = NONE;
        }

        void set(long long value) {
          i_value = value;
          p_type = INTEGER;
        }

        void set(double value) {
          d_value = value;
          p_type = DOUBLE;
        }

        void set(bool value) {
          b_value = value;
          p_type = BOOL;
        }

        void set(std::string value) {
          s_value = value;
          p_type = STRING;
        }

        void set(PDFObjectCastPtr<PDFArray> value) {
          a_value = value;
          p_type = PDFARRAY;
        }

        void set(const char* value) {
          s_value = value;
          p_type = STRING;
        }

        void set(const pdf_value_t& m) {
          p_type = m.p_type;
          switch(p_type) {
            case NONE: {
              break;
            }
            case INTEGER: {
              i_value = m.i_value;
              break;
            }
            case DOUBLE: {
              d_value = m.d_value;
              break;
            }
            case BOOL: {
              b_value = m.b_value;
              break;
            }
            case STRING: {
              s_value = m.s_value;
              break;
            }
            case PDFARRAY: {
              a_value = m.a_value;
            }
            default: {
              break;
            }
          }
        }


        pdf_value_t() {
          p_type = NONE;
        }

        pdf_value_t(const pdf_value_t& value) {
          set(value);
        }

        pdf_value_t(long long value) {
          set(value);
        }

        pdf_value_t(double value) {
          set(value);
        }

        pdf_value_t(std::string value) {
          set(value);
        }

        pdf_value_t(const char* value) {
          set(value);
        }

        pdf_value_t(bool value) {
          set(value);
        }

        pdf_value_t(PDFObjectCastPtr<PDFArray> value) {
          set(value);
        }

        std::string type() {
          switch(p_type) {
            case NONE: {
              return "none";
            }
            case INTEGER: {
              return "int";
            }
            case DOUBLE: {
              return "double";
            }
            case BOOL: {
              return "bool";
            }
            case STRING: {
              return "string";
            }
            case PDFARRAY: {
              return "pdfarray";
            }
            default: {
              return "none";
            }
          }
        }

        bool operator==(const pdf_value_t& m) {
          if(p_type != m.p_type) {
            return false;
          }

          switch(p_type) {
            case NONE: {
              return true;
            }
            case INTEGER: {
              return i_value == m.i_value;
            }
            case DOUBLE: {
              return d_value == m.d_value;
            }
            case BOOL: {
              return b_value == m.b_value;
            }
            case STRING: {
              return s_value == m.s_value;
            }
            case PDFARRAY: {
              return a_value.GetPtr() == m.a_value.GetPtr();
            }
            default: {
              return false;
            }
          }
        }

        bool operator!=(const pdf_value_t& m) {
          if(p_type != m.p_type) {
            return true;
          }

          switch(p_type) {
            case NONE: {
              return false;
            }
            case INTEGER: {
              return i_value != m.i_value;
            }
            case DOUBLE: {
              return d_value != m.d_value;
            }
            case BOOL: {
              return b_value != m.b_value;
            }
            case STRING: {
              return s_value != m.s_value;
            }
            case PDFARRAY: {
              return a_value.GetPtr() != m.a_value.GetPtr();
            }
            default: {
              return false;
            }
          }
        }

        void operator=(long long value) {
          set(value);
        }

        void operator=(double value) {
          set(value);
        }

        void operator=(bool value) {
          set(value);
        }

        void operator=(std::string value) {
          set(value);
        }

        void operator=(const char* value) {
          set(value);
        }

        void operator=(const pdf_value_t& value) {
          set(value);
        }

        void operator=(PDFObjectCastPtr<PDFArray> value) {
          set(value);
        }

        long long ToInteger() {
          switch(p_type) {
            case NONE: {
              return 0;
            }
            case INTEGER: {
              return i_value;
            }
            case DOUBLE: {
              return (long long)d_value;
            }
            case BOOL: {
              return (long long)b_value;
            }
            case STRING: {
              return strtoll(s_value.c_str(), NULL, 0);
            }
            case PDFARRAY: {
              return 0;
            }
            default: {
              return 0;
            }
          }
        }

        double ToDouble() {
          switch(p_type) {
            case NONE: {
              return 0.0;
            }
            case INTEGER: {
              return (double)i_value;
            }
            case DOUBLE: {
              return d_value;
            }
            case BOOL: {
              return (double)b_value;
            }
            case STRING: {
              return strtod(s_value.c_str(), NULL);
            }
            case PDFARRAY: {
              return 0.0;
            }
            default: {
              return 0.0;
            }
          }
        }

        bool ToBool() {
          switch(p_type) {
            case NONE: {
              return false;
            }
            case INTEGER: {
              return (bool)i_value;
            }
            case DOUBLE: {
              return (bool)d_value;
            }
            case BOOL: {
              return b_value;
            }
            case STRING: {
              return s_value.size() != 0;
            }
            case PDFARRAY: {
              return a_value.GetPtr() != NULL;
            }
            default: {
              return false;
            }
          }
        }

        std::string ToString() {
          switch(p_type) {
            case NONE: {
              return "";
            }
            case INTEGER: {
              return std::to_string(i_value);
            }
            case DOUBLE: {
              return std::to_string(d_value);
            }
            case BOOL: {
              return b_value ? "true" : "false";
            }
            case STRING: {
              return s_value;
            }
            case PDFARRAY: {
              return "";
            }
            default: {
              return "";
            }
          }
        }

        PDFObjectCastPtr<PDFArray> ToPDFArray() {
          switch(p_type) {
            case NONE: {
              return NULL;
            }
            case INTEGER: {
              return NULL;
            }
            case DOUBLE: {
              return NULL;
            }
            case BOOL: {
              return NULL;
            }
            case STRING: {
              return NULL;
            }
            case PDFARRAY: {
              return a_value;
            }
            default: {
              return NULL;
            }
          }
        }
    };

    typedef struct {
      bool debug;
      AbstractContentContext::TextOptions* defaultTextOptions;
    } options_t;

    typedef struct {
      PDFWriter& writer;
      PDFParser& reader;
      PDFDocumentCopyingContext* copyingContext;
      ObjectsContext& objectsContext;
      std::map<std::string, pdf_value_t> data;
      PDFObjectCastPtr<PDFDictionary> acroformDict;
      options_t options;
    } handles_t;

    typedef struct {
      bool existing;
      ObjectIDType id;
      PDFObject* field;
    } field_ref_t;

  private:
    /**
     * a wonderfully reusable method to recreate a dict without all the keys that we want to change
     * note that it starts writing a dict, but doesn't finish it. your job
     */
    DictionaryContext* startModifiedDictionary(handles_t handles, PDFObjectCastPtr<PDFDictionary> originalDict, std::vector<std::string> excludedKeys) {
      DictionaryContext* newDict = handles.objectsContext.StartDictionary();

      MapIterator<PDFNameToPDFObjectMap> it = originalDict->GetIterator();
      PDFObjectCastPtr<PDFName> key;
      PDFObject* value;

      while(it.MoveNext()) {
        key = it.GetKey();
        value = it.GetValue();
        if(std::find(excludedKeys.begin(), excludedKeys.end(), key->GetValue()) != excludedKeys.end()) {
          continue;
        }

        newDict->WriteKey(key->GetValue());
        handles.copyingContext->CopyDirectObjectAsIs(value);
      }
      return newDict;
    }

    void defaultTerminalFieldWrite(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary) {
      // default write of ending field. no reason to recurse to kids
      handles.copyingContext->CopyDirectObjectAsIs(fieldDictionary.GetPtr());
      handles.objectsContext.EndIndirectObject();
    }

    /**
     * Update radio button value. look for the field matching the value, which should be an index.
     * Set its ON appearance as the value, and set all radio buttons appearance to off, but the selected one which should be on
     */
    void updateOptionButtonValue(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary, pdf_value_t value) {
      bool isWidget = false;
      PDFObjectCastPtr<PDFName> subtype = fieldDictionary->QueryDirectObject("Subtype");
      isWidget = (subtype != NULL && subtype->GetValue() == "Widget");


      if (isWidget || fieldDictionary->Exists("Kids") == false) {
        // this radio button has just one option and its in the widget. also means no kids
        DictionaryContext* modifiedDict = startModifiedDictionary(handles, fieldDictionary, { "V", "AS" });
        std::string appearanceName;
        if (value.type() == "none") {
          // false is easy, just write '/Off' as the value and as the appearance stream
          appearanceName = "Off";
        } else {
          // grab the non off value. that should be the yes one
          PDFObjectCastPtr<PDFDictionary> apDictionary = handles.reader.QueryDictionaryObject(fieldDictionary.GetPtr(), "AP");
          PDFObjectCastPtr<PDFDictionary> nAppearances = handles.reader.QueryDictionaryObject(apDictionary.GetPtr(), "N");
          MapIterator<PDFNameToPDFObjectMap> it = nAppearances->GetIterator();
          PDFObjectCastPtr<PDFName> key;
          while(it.MoveNext()) {
            key = it.GetKey();
            if(key->GetValue() != "Off") {
              appearanceName = key->GetValue();
              break;
            }
          }
        }

        modifiedDict->WriteKey("V");
        modifiedDict->WriteNameValue(appearanceName);
        modifiedDict->WriteKey("AS");
        modifiedDict->WriteNameValue(appearanceName);

        handles.objectsContext.EndDictionary(modifiedDict);
        handles.objectsContext.EndIndirectObject();
      } else {
        // Field. this would mean that there's a kid array, and there are offs and ons to set
        DictionaryContext* modifiedDict = startModifiedDictionary(handles, fieldDictionary, {"V", "Kids"});
        PDFObjectCastPtr<PDFArray> kidsArray = handles.reader.QueryDictionaryObject(fieldDictionary.GetPtr(), "Kids");

        std::string appearanceName;
        if (value.ToString() == "none") {
          // false is easy, just write '/Off' as the value and as the appearance stream
          appearanceName = "Off";
        } else {
          // grab the non off value. that should be the yes one
          PDFObjectCastPtr<PDFDictionary> widgetDictionary = handles.reader.QueryArrayObject(kidsArray.GetPtr(), value.ToInteger());
          PDFObjectCastPtr<PDFDictionary> apDictionary = handles.reader.QueryDictionaryObject(widgetDictionary.GetPtr(), "AP");
          PDFObjectCastPtr<PDFDictionary> nAppearances = handles.reader.QueryDictionaryObject(apDictionary.GetPtr(), "N");
          MapIterator<PDFNameToPDFObjectMap> it  = nAppearances->GetIterator();
          PDFObjectCastPtr<PDFName> key;
          while(it.MoveNext()) {
            key = it.GetKey();
            if(key->GetValue() != "Off") {
              appearanceName = key->GetValue();
              break;
            }
          }
        }

        // set the V value on the new field dictionary
        modifiedDict->WriteKey("V");
        modifiedDict->WriteNameValue(appearanceName);

        // write the Kids key before we write the kids array
        modifiedDict->WriteKey("Kids");

        // write the kids array, similar to writeFilledFields, but knowing that these are widgets and that AS needs to be set
        std::vector<field_ref_t> fieldsReferences = writeKidsAndEndObject(handles, modifiedDict, kidsArray);

        // recreate widget kids, turn on or off based on their relation to the target value
        for(size_t i = 0; i < fieldsReferences.size(); i++) {
          field_ref_t fieldReference = fieldsReferences[i];
          PDFObjectCastPtr<PDFDictionary> sourceField = NULL;

          if(fieldReference.existing) {
            handles.objectsContext.StartModifiedIndirectObject(fieldReference.id);
            sourceField = handles.reader.ParseNewObject(fieldReference.id);
          } else {
            handles.objectsContext.StartNewIndirectObject(fieldReference.id);
            sourceField = fieldReference.field;
          }

          DictionaryContext* modifiedFieldDict = startModifiedDictionary(handles, sourceField, { "AS" });
          if (value.ToInteger() == i) {
            // this widget should be on
            modifiedFieldDict->WriteKey("AS"); //doen
            modifiedFieldDict->WriteNameValue(appearanceName); // note that we have saved it earlier
          } else {
            // this widget should be off
            modifiedFieldDict->WriteKey("AS");
            modifiedFieldDict->WriteNameValue("Off");
          }
          // finish
          handles.objectsContext.EndDictionary(modifiedFieldDict);
          handles.objectsContext.EndIndirectObject();
        }
      }
    }

    std::string getOriginalTextFieldAppearanceStreamCode(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary) {
      // get the single appearance stream for the text, field. we'll use it to recreate the new one
      bool appearanceInField = fieldDictionary->Exists("Subtype");
      if(appearanceInField) {
        bool is_widget;
        PDFObjectCastPtr<PDFName> subtype = fieldDictionary->QueryDirectObject("Subtype");
        appearanceInField = (subtype->GetValue() == "Widget" || !fieldDictionary->Exists("Kids"));
      }

      PDFObjectCastPtr<PDFDictionary> appearanceParent = NULL;
      if(appearanceInField) {
        appearanceParent = fieldDictionary;
      } else {
        if(fieldDictionary->Exists("Kids")) {
          PDFObjectCastPtr<PDFArray> kidsArray = handles.reader.QueryDictionaryObject(fieldDictionary.GetPtr(), "Kids");
          if(kidsArray->GetLength() > 0) {
            appearanceParent = handles.reader.QueryArrayObject(kidsArray.GetPtr(), 0);
          }
        }
      }

      if(!appearanceParent) {
        return _NULL_;
      }

      if(!appearanceParent->Exists("AP"))
        return _NULL_;

      PDFObject* m =  handles.reader.QueryDictionaryObject(appearanceParent.GetPtr(), "AP");
      PDFObjectCastPtr<PDFDictionary> appearance = handles.reader.QueryDictionaryObject(appearanceParent.GetPtr(), "AP");
      if(!appearance->Exists("N"))
        return _NULL_;

      PDFObjectCastPtr<PDFStreamInput> appearanceXObject = handles.reader.QueryDictionaryObject(appearance.GetPtr(), "N");
      return readStreamToString(handles, appearanceXObject);
    }

    void writeAppearanceXObjectForText(handles_t handles, ObjectIDType formId, PDFObjectCastPtr<PDFDictionary> fieldsDictionary, pdf_value_t text, std::map<std::string, pdf_value_t> inheritedProperties) {
      PDFObjectCastPtr<PDFArray> rect = handles.reader.QueryDictionaryObject(fieldsDictionary.GetPtr(), "Rect");

      PDFObjectCastPtr<PDFName> _da;
      std::string da;
      if((_da = fieldsDictionary->QueryDirectObject("DA")) != NULL) {
        da = _da->GetValue();
      } else {
        da = inheritedProperties["DA"].ToString();
      }

      PDFObjectCastPtr<PDFInteger> _q;
      long long q;
      if((_q = fieldsDictionary->QueryDirectObject("Q")) != NULL) {
        q = _q->GetValue();
      } else {
        q = inheritedProperties["Q"].ToInteger();
      }

      if(handles.options.debug) {
        printf("creating new appearance with:\n");
          printf("da = %s\n", da.c_str());
          printf("q = %lli\n", q);
          //printf("fieldsDictionary =", fieldsDictionary.toJSObject());
          //printf("inheritedProperties =", inheritedProperties);
          printf("text = %s\n", text.ToString().c_str());
      }

      std::string originalAppearanceContent = getOriginalTextFieldAppearanceStreamCode(handles, fieldsDictionary);
      std::string before = "";
      std::string after = "";

      std::string lookfor_bmc = "/Tx BMC";
      std::string lookfor_emc = "EMC";

      if(originalAppearanceContent != _NULL_) {
        size_t pre = originalAppearanceContent.find(lookfor_bmc);
          if(pre != std::string::npos) {
            before = originalAppearanceContent.substr(0, pre);
            size_t post = originalAppearanceContent.find(lookfor_emc, pre + lookfor_bmc.size());
              if(post != std::string::npos) {
                after = originalAppearanceContent.substr(post + lookfor_emc.size());
              }
          } else {
            before = originalAppearanceContent;
          }
      }

      double upper_right_x = 0.0;
      double lower_left_x = 0.0;
      double upper_right_y = 0.0;
      double lower_left_y = 0.0;

      PDFObject* t;
      t = handles.reader.QueryArrayObject(rect.GetPtr(), 0);
      if(t->GetType() == PDFDictionary::ePDFObjectInteger) {
        lower_left_x = ((PDFObjectCastPtr<PDFInteger>)t)->GetValue();

      } else if(t->GetType() == PDFDictionary::ePDFObjectReal){
        lower_left_x = ((PDFObjectCastPtr<PDFReal>)t)->GetValue();
      }

      t = handles.reader.QueryArrayObject(rect.GetPtr(), 1);
      if(t->GetType() == PDFDictionary::ePDFObjectInteger) {
        lower_left_y = ((PDFObjectCastPtr<PDFInteger>)t)->GetValue();

      } else if(t->GetType() == PDFDictionary::ePDFObjectReal){
        lower_left_y = ((PDFObjectCastPtr<PDFReal>)t)->GetValue();
      }

      t = handles.reader.QueryArrayObject(rect.GetPtr(), 2);
      if(t->GetType() == PDFDictionary::ePDFObjectInteger) {
        upper_right_x = ((PDFObjectCastPtr<PDFInteger>)t)->GetValue();

      } else if(t->GetType() == PDFDictionary::ePDFObjectReal){
        upper_right_x = ((PDFObjectCastPtr<PDFReal>)t)->GetValue();
      }

      t = handles.reader.QueryArrayObject(rect.GetPtr(), 3);
      if(t->GetType() == PDFDictionary::ePDFObjectInteger) {
        upper_right_y = ((PDFObjectCastPtr<PDFInteger>)t)->GetValue();

      } else if(t->GetType() == PDFDictionary::ePDFObjectReal){
        upper_right_y = ((PDFObjectCastPtr<PDFReal>)t)->GetValue();
      }

      double boxWidth = upper_right_x - lower_left_x;
      double boxHeight = upper_right_y - lower_left_y;

      PDFFormXObject* xobjectForm = handles.writer.StartFormXObject(PDFRectangle(0, 0, boxWidth, boxHeight), formId);

      // If default text options setup, use them to determine the text appearance. including quad support, horizontal centering etc.
      // Otherwise, use naive method: Will use Tj with "code" encoding to write the text, assuming encoding should work (??). if it won't i need real fonts here
      // and DA is not gonna be useful. so for now let's use as is.
      // For the same reason i'm not support Quad, as well

      // Should be able to parse the following from the DA, and map to system font
      // temporarily, let user input the values
      AbstractContentContext::TextOptions* textOptions = handles.options.defaultTextOptions;

      if(textOptions != NULL) {
        // grab text dimensions for quad support and vertical centering
        PDFUsedFont::TextMeasures textDimensions = textOptions->font->CalculateTextDimensions(text.ToString(), textOptions->fontSize);

        // vertical centering
        double yPos = (boxHeight - textDimensions.height) / 2;
        // horizontal pos per quad
        long long quad = q || 0;

        double xPos = 0;
        switch(quad) {
          case 0:
            // left align
            xPos = 0;
            break;
          case 1:
            // center
            xPos = (boxWidth - textDimensions.width) / 2;
            break;
          case 2:
            // right align
            xPos = (boxWidth - textDimensions.width);
        }

        XObjectContentContext* xobjectFormContext = xobjectForm->GetContentContext();
          xobjectFormContext->WriteFreeCode(before);
          xobjectFormContext->WriteFreeCode("/Tx BMC\r\n");
          xobjectFormContext->q();
          xobjectFormContext->WriteText(xPos, yPos, text.ToString(), *textOptions);
          xobjectFormContext->Q();
          xobjectFormContext->WriteFreeCode("EMC");
          xobjectFormContext->WriteFreeCode(after);;
      } else {
        // Naive form, no quad support...and text may not show and may be mispositioned
        XObjectContentContext* xobjectFormContext = xobjectForm->GetContentContext();
        xobjectFormContext->WriteFreeCode(before);
        xobjectFormContext->WriteFreeCode("/Tx BMC\r\n");
        xobjectFormContext->q();
        xobjectFormContext->BT();
        xobjectFormContext->WriteFreeCode(da + "\r\n");
        xobjectFormContext->Tj(text.ToString());
        xobjectFormContext->ET();
        xobjectFormContext->Q();
        xobjectFormContext->WriteFreeCode("EMC");
        xobjectFormContext->WriteFreeCode(after);
      }

      if(handles.acroformDict->Exists("DR")) {
        // copy all but the keys that exist already
        PDFObjectCastPtr<PDFDictionary> dr = handles.reader.QueryDictionaryObject(handles.acroformDict.GetPtr(), "DR");

        MapIterator<PDFNameToPDFObjectMap> it = dr->GetIterator();
        RefCountPtr<PDFName> key;
        PDFObject* value;
        DictionaryContext* page_out_dic = handles.writer.GetObjectsContext().StartDictionary();
        while(it.MoveNext()) {
          key = it.GetKey();
          value = it.GetValue();
          if(key->GetValue() != "ProcSet" && (textOptions == NULL || key->GetValue() != "Font")) {
            page_out_dic->WriteKey(key->GetValue());
            handles.copyingContext->CopyDirectObjectAsIs(value);
          }
        }
      }

      handles.writer.EndFormXObject(xobjectForm);
    }

    #define BUFFER_SIZE 10000
    std::string readStreamToString(handles_t handles, PDFObjectCastPtr<PDFStreamInput> stream) {
      Byte readData[BUFFER_SIZE];
      std::string buff = "";
      IByteReader* readStream = handles.reader.StartReadingFromStream(stream.GetPtr());

      while(readStream->NotEnded()) {
        IOBasicTypes::LongBufferSizeType read = readStream->Read(readData, BUFFER_SIZE);
        for(IOBasicTypes::LongBufferSizeType i = 0; i < read; i++) {
          UnicodeString u;
          u.FromUTF16((char*)readData);
          EStatusCodeAndString result = u.ToUTF8();
          buff += result.second;
        }
      }
      return buff;
    }

    void writeFieldWithAppearanceForText(handles_t handles, DictionaryContext* targetFieldDict, PDFObjectCastPtr<PDFDictionary> sourceFieldDictionary, bool appearanceInField, pdf_value_t textToWrite, std::map<std::string, pdf_value_t> inheritedProperties) {
      // determine how to write appearance
      ObjectIDType newAppearanceFormId = handles.objectsContext.GetInDirectObjectsRegistry().AllocateNewObjectID();
      if(appearanceInField) {
        // Appearance in field - so write appearance dict in field
        targetFieldDict->WriteKey("AP");

        DictionaryContext* apDict = handles.objectsContext.StartDictionary();
        apDict->WriteKey("N");
        apDict->WriteObjectReferenceValue(newAppearanceFormId);
        handles.objectsContext.EndDictionary(apDict);
        handles.objectsContext.EndDictionary(targetFieldDict);
        handles.objectsContext.EndIndirectObject();
      } else {
        // finish the field object
        handles.objectsContext.EndDictionary(targetFieldDict);
        handles.objectsContext.EndIndirectObject();

        // write in kid (there should be just one)
        PDFObjectCastPtr<PDFArray> kidsArray = handles.reader.QueryDictionaryObject(sourceFieldDictionary.GetPtr(), "Kids");
        std::vector<field_ref_t> fieldsReferences = writeKidsAndEndObject(handles, targetFieldDict, kidsArray);

        // recreate widget kid, with new stream reference
        field_ref_t fieldReference = fieldsReferences[0];

        PDFObjectCastPtr<PDFDictionary> sourceField;
        if(fieldReference.existing) {
          handles.objectsContext.StartModifiedIndirectObject(fieldReference.id);
          sourceField = handles.reader.ParseNewObject(fieldReference.id);
        } else {
          handles.objectsContext.StartNewIndirectObject(fieldReference.id);
          sourceField = fieldReference.field;
        }

        DictionaryContext* modifiedDict = startModifiedDictionary(handles, sourceField, {"AP"});
        modifiedDict->WriteKey("AP");

        DictionaryContext* apDict = handles.objectsContext.StartDictionary();
        apDict->WriteKey("N");
        apDict->WriteObjectReferenceValue(newAppearanceFormId);
        handles.objectsContext.EndDictionary(apDict);
        handles.objectsContext.EndDictionary(modifiedDict);
        handles.objectsContext.EndIndirectObject();
      }

      // write the new stream xobject
      writeAppearanceXObjectForText(handles, newAppearanceFormId, sourceFieldDictionary, textToWrite, inheritedProperties);
    }

    void updateTextValue(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary, pdf_value_t value, bool isRich, std::map<std::string, pdf_value_t> inheritedProperties) {
      std::vector<std::string> fieldsToRemove;
      bool appearanceInField = fieldDictionary->Exists("Subtype");
      if(appearanceInField) {
        PDFObjectCastPtr<PDFName> subtype = fieldDictionary->QueryDirectObject("Subtype");
        appearanceInField = (subtype->GetValue() == "Widget" || !fieldDictionary->Exists("Kids"));
      }
      fieldsToRemove.push_back("V");

      if(appearanceInField) {
        // add skipping AP if in field (and not in a child widget)
        fieldsToRemove.push_back("AP");
      }
      if(isRich) {
        // skip RV if rich
        fieldsToRemove.push_back("RV");
      }

      DictionaryContext* modifiedDict = startModifiedDictionary(handles, fieldDictionary, fieldsToRemove);

      // start with value, setting both plain value and rich value
      modifiedDict->WriteKey("V");
      modifiedDict->WriteLiteralStringValue(PDFHexString(value.ToString()));

      if(isRich) {
        modifiedDict->WriteKey("RV");
        modifiedDict->WriteLiteralStringValue(PDFHexString(value.ToString()));
      }

      writeFieldWithAppearanceForText(handles, modifiedDict, fieldDictionary, appearanceInField, value, inheritedProperties);
    }

    void updateChoiceValue(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary, pdf_value_t value, std::map<std::string, pdf_value_t> inheritedProperties) {
      bool appearanceInField = fieldDictionary->Exists("Subtype");
      if(appearanceInField) {
        PDFObjectCastPtr<PDFName> subtype = fieldDictionary->QueryDirectObject("Subtype");
        appearanceInField = (subtype->GetValue() == "Widget" || !fieldDictionary->Exists("Kids"));
      }

      std::vector<std::string> fieldsToRemove = { "V" };

      if (appearanceInField) {
        // add skipping AP if in field (and not in a child widget)
        fieldsToRemove.push_back("AP");
      }

      DictionaryContext* modifiedDict = startModifiedDictionary(handles, fieldDictionary, fieldsToRemove);

      // start with value, setting per one or multiple selection. also choose the text to write in appearance
      std::string textToWrite;
      if(value.type() == "string") {
        // one option
        modifiedDict->WriteKey("V");
        modifiedDict->WriteLiteralStringValue(PDFTextString(value.ToString()).ToString());
        textToWrite = value.ToString();
      } else {
        // multiple options
        modifiedDict->WriteKey("V");
        handles.objectsContext.StartArray();
        PDFObjectCastPtr<PDFArray> array = value.ToPDFArray();
        SingleValueContainerIterator<PDFObjectVector> it = array->GetIterator();
        bool first = true;
        textToWrite = "";
        while(it.MoveNext()) {
          PDFObjectCastPtr<PDFLiteralString> field = it.GetItem();
          if(first) {
            first = false;
            textToWrite = field->GetValue();
          }
          handles.objectsContext.WriteLiteralString(field->GetValue());
        }
        handles.objectsContext.EndArray();
      }

      writeFieldWithAppearanceForText(handles, modifiedDict, fieldDictionary, appearanceInField, textToWrite, inheritedProperties);
    }

    /**
     * Update a field. splits to per type functions
     */
    void updateFieldWithValue(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary, pdf_value_t value, std::map<std::string, pdf_value_t> inheritedProperties) {
      // Update a field with value. There is a logical assumption made here:
      // This must be a terminal field. meaning it is a field, and it either has no kids, it also holding
      // Widget data or that it has one or more kids defining its widget annotation(s). Normally it would be
      // One but in the case of a radio button, where there's one per option.
      std::string localFieldType = "";
      std::string fieldType = "";
      long long   localFlags = 0;
      long long   flags = 0;

      PDFObjectCastPtr<PDFName> ft = fieldDictionary->QueryDirectObject("FT");
      if(ft != NULL) {
        fieldType = ft->GetValue();
      } else {
        fieldType = inheritedProperties["FT"].ToString();
      }

      PDFObjectCastPtr<PDFInteger> ff = fieldDictionary->QueryDirectObject("Ff");
      if(ff != NULL) {
        flags = ff->GetValue();
      } else {
        flags = inheritedProperties["Ff"].ToInteger();
      }

      // the rest is fairly type dependent, so let's check the type
      if(fieldType == "Btn") {
        if((flags >> 16) & 1) {
          // push button. can't write a value. forget it.
          defaultTerminalFieldWrite(handles, fieldDictionary);
        } else {
          // checkbox or radio button
          updateOptionButtonValue(handles,fieldDictionary,(((flags >> 15) & 1) != 0) ? value : (value.ToBool() ? pdf_value_t(false): pdf_value_t()));
        }
      } else if(fieldType == "Tx") {
        // rich or plain text
        updateTextValue(handles, fieldDictionary, value, (flags >> 25) & 1, inheritedProperties);
      } else if(fieldType == "Ch") {
        updateChoiceValue(handles, fieldDictionary, value, inheritedProperties);
      } else if(fieldType == "Sig") {
        // signature, ain't handling that. should return or throw an error sometimes
        defaultTerminalFieldWrite(handles, fieldDictionary);
      } else {
        // in case there's a fault and there's no type, or it's irrelevant
        defaultTerminalFieldWrite(handles, fieldDictionary);
      }
    }

    void writeFieldAndKids(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary, std::map<std::string, pdf_value_t> inheritedProperties, std::string baseFieldName) {
      // this field or widget doesn't need value rewrite. but its kids might. so write the dictionary as is, dropping kids.
      // write them later and recurse.

      DictionaryContext* modifiedFieldDict = startModifiedDictionary(handles, fieldDictionary, { "Kids" });
      // if kids exist, continue to them for extra filling!
      PDFObjectCastPtr<PDFArray> kids = handles.reader.QueryDictionaryObject(fieldDictionary.GetPtr(), "Kids");

      if(kids != NULL) {
        std::map<std::string, pdf_value_t> localEnv;

        // prep some inherited values and push env
        if (fieldDictionary->Exists("FT"))
          localEnv["FT"] = ((PDFObjectCastPtr<PDFName>) fieldDictionary->QueryDirectObject("FT"))->GetValue();
        if (fieldDictionary->Exists("Ff"))
          localEnv["Ff"] = ((PDFObjectCastPtr<PDFInteger>) fieldDictionary->QueryDirectObject("Ff"))->GetValue();
        if (fieldDictionary->Exists("DA"))
          localEnv["DA"] = ((PDFObjectCastPtr<PDFName>) fieldDictionary->QueryDirectObject("DA"))->GetValue();
        if (fieldDictionary->Exists("Q"))
          localEnv["Q"] = ((PDFObjectCastPtr<PDFInteger>) fieldDictionary->QueryDirectObject("Q"))->GetValue();
        if (fieldDictionary->Exists("Opt"))
          localEnv["Opt"] = (PDFObjectCastPtr<PDFArray>) fieldDictionary->QueryDirectObject("Opt");

        modifiedFieldDict->WriteKey("Kids");
        // recurse to kids. note that this will take care of ending this object
        localEnv.insert(inheritedProperties.begin(), inheritedProperties.end());
        writeFilledFields(handles, modifiedFieldDict, kids, localEnv, baseFieldName + ".");
      } else {
        // no kids, can finish object now
        handles.objectsContext.EndDictionary(modifiedFieldDict);
        handles.objectsContext.EndIndirectObject();
      }
    }

    /**
     * writes a single field. will fill with value if found in data.
     * assuming that's in indirect object and having to write the dict,finish the dict, indirect object and write the kids
     */
    void writeFilledField(handles_t handles, PDFObjectCastPtr<PDFDictionary> fieldDictionary, std::map<std::string, pdf_value_t> inheritedProperties, std::string baseFieldName) {
      std::string fullName = "";
      PDFObject* value;
      if((value = fieldDictionary->QueryDirectObject("T")) != NULL) {
        ParsedPrimitiveHelper helper(value);
        fullName = baseFieldName + helper.ToString();
      } else {
        fullName = baseFieldName;
      }

      // Based on the fullName we can now determine whether the field has a value that needs setting
      auto found = handles.data.find(fullName);
      if(found != handles.data.end()) {
        // We got a winner! write with updated value
        updateFieldWithValue(handles, fieldDictionary, handles.data[fullName], inheritedProperties);
      } else {
        // Not yet. write and recurse to kids
        writeFieldAndKids(handles, fieldDictionary, inheritedProperties, fullName);
      }
    }

    /**
     * Write kids array converting each direct kids to an indirect one
     */
    std::vector<field_ref_t> writeKidsAndEndObject(handles_t handles, DictionaryContext* parentDict, PDFObjectCastPtr<PDFArray> kidsArray) {
      std::vector<field_ref_t> fieldsReferences;

      handles.objectsContext.StartArray();
      SingleValueContainerIterator<PDFObjectVector> it = kidsArray->GetIterator();
      while(it.MoveNext()) {
        PDFObject* field = it.GetItem();
        if(field->GetType() == PDFDictionary::ePDFObjectIndirectObjectReference) {
          // existing reference, keep as is
          handles.copyingContext->CopyDirectObjectAsIs(field);
          fieldsReferences.push_back({true, ((PDFObjectCastPtr<PDFIndirectObjectReference>)field)->mObjectID, NULL});
        } else {
          ObjectIDType newFieldObjectId = handles.objectsContext.GetInDirectObjectsRegistry().AllocateNewObjectID();
          // direct object, recreate as reference
          fieldsReferences.push_back({false, newFieldObjectId, field});
          handles.objectsContext.WriteIndirectObjectReference(newFieldObjectId);
        }
      }
      handles.objectsContext.EndArray(eTokenSeparatorEndLine);
      handles.objectsContext.EndDictionary(parentDict);
      handles.objectsContext.EndIndirectObject();
      return fieldsReferences;
    }

    /**
     * write fields/kids array of dictionary. make sure all become indirect, for the sake of simplicity,
     * which is why it gets to take care of finishing the writing of the said dict
     */
    void writeFilledFields(handles_t handles, DictionaryContext* parentDict, PDFObjectCastPtr<PDFArray> fields, std::map<std::string, pdf_value_t> inheritedProperties, std::string baseFieldName) {
      std::vector<field_ref_t> fieldsReferences = writeKidsAndEndObject(handles, parentDict, fields);
      // now recreate the fields, filled this time (and down the recursion hole...)
      for(size_t i = 0; i < fieldsReferences.size(); i++) {
        if(fieldsReferences[i].existing) {
          handles.objectsContext.StartModifiedIndirectObject(fieldsReferences[i].id);
          PDFObjectCastPtr<PDFDictionary> refdict = handles.reader.ParseNewObject(fieldsReferences[i].id);
          writeFilledField(handles, refdict, inheritedProperties, baseFieldName);
        } else {
          handles.objectsContext.StartNewIndirectObject(fieldsReferences[i].id);
          PDFObjectCastPtr<PDFDictionary> refdict = fieldsReferences[i].field;
          writeFilledField(handles, refdict, inheritedProperties, baseFieldName);
        }
      }
    }

    /**
     * Write a filled form dictionary, and its subordinate fields.
     * assumes in an indirect object, so will finish it
     */
    void writeFilledForm(handles_t handles, PDFObjectCastPtr<PDFDictionary> acroformDict) {
      DictionaryContext* modifiedAcroFormDict = startModifiedDictionary(handles, acroformDict, { "Fields" });

      PDFObjectCastPtr<PDFArray> fields = NULL;
      if(acroformDict->Exists("Fields")) {
        fields = handles.reader.QueryDictionaryObject(acroformDict.GetPtr(), "Fields");
      }

      if(fields != NULL) {
        modifiedAcroFormDict->WriteKey("Fields");
        writeFilledFields(handles, modifiedAcroFormDict, fields, { }, ""); // will also take care of finishing the dictionary and indirect object, so no need to finish after
      } else {
        handles.objectsContext.EndDictionary(modifiedAcroFormDict);
        handles.objectsContext.EndIndirectObject();
      }
      fflush(stdout);
    }

  public:
    void fillForm(PDFWriter& writer, std::map<std::string, pdf_value_t> data, options_t options = { false, NULL }) {
      PDFParser& reader = writer.GetModifiedFileParser();

      PDFObjectCastPtr<PDFDictionary> catalogDict = reader.QueryDictionaryObject(reader.GetTrailer(), "Root");

      if(catalogDict == NULL) {
        throw "Root not found";
      }

      PDFObject* acroformInCatalog = catalogDict->QueryDirectObject("AcroForm");

      if(catalogDict == NULL) {
        throw "AcroForm not found 1";
      }

      PDFDocumentCopyingContext* copyingContext = writer.CreatePDFCopyingContextForModifiedFile();
      ObjectsContext& objectsContext = writer.GetObjectsContext();

      PDFObject* acroformDict = reader.QueryDictionaryObject(catalogDict.GetPtr(), "AcroForm");
      if(acroformDict == NULL) {
        throw "AcroForm not found 2";
      }

      handles_t handles = {
        .writer = writer,
        .reader = reader,
        .copyingContext = copyingContext,
        .objectsContext = objectsContext,
        .data = data,
        .acroformDict = acroformDict,
        .options = options,
      };

      // recreate a copy of the existing form, which we will fill with data.
      if(acroformInCatalog->GetType() == PDFDictionary::ePDFObjectIndirectObjectReference) {
        // if the form is a referenced object, modify it
        ObjectIDType acroformObjectId = (PDFObjectCastPtr<PDFIndirectObjectReference>(acroformInCatalog))->mObjectID;
        objectsContext.StartModifiedIndirectObject(acroformObjectId);

        writeFilledForm(handles, acroformDict);
      } else {
        // otherwise, recreate the form as an indirect child (this is going to be a general policy, we're making things indirect. it's simpler), and recreate the catalog
        ObjectIDType catalogObjectId = (PDFObjectCastPtr<PDFIndirectObjectReference>(reader.GetTrailer()->QueryDirectObject("Root")))->mObjectID;
        ObjectIDType newAcroformObjectId = objectsContext.GetInDirectObjectsRegistry().AllocateNewObjectID();

        // recreate the catalog with form pointing to new reference
        objectsContext.StartModifiedIndirectObject(catalogObjectId);
        DictionaryContext* modifiedCatalogDictionary = startModifiedDictionary(handles, catalogDict, {"AcroForm"});

        modifiedCatalogDictionary->WriteKey("AcroForm");
        modifiedCatalogDictionary->WriteObjectReferenceValue(newAcroformObjectId);
        objectsContext.EndDictionary(modifiedCatalogDictionary);
        objectsContext.EndIndirectObject();

        // now create the new form object
        objectsContext.StartNewIndirectObject(newAcroformObjectId);

        writeFilledForm(handles, acroformDict);
      }
    }
};


#endif //__PDF_FORM_FILL_H__
