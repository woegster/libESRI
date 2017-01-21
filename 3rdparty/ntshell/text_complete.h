//Costum extension by woegster, not part of the original ntshell
#ifndef TEXT_COMPLETE_H
#define TEXT_COMPLETE_H

#include "ntconf.h"

/**
 * @brief Maximum length per complete command.
 */
#define TEXTCOMPLETE_MAXLEN  32

/**
 * @brief Depth of the history.
 */
#define TEXTCOMPLE_MAXENTRIES 8

/**
 * @brief Structure of the text history module.
 */
typedef struct {
    /**
     * @brief buffer for known commands.
     */
    char complete[TEXTCOMPLETE_MAXLEN * TEXTCOMPLE_MAXENTRIES];

    /**
    * @brief index of highest entrie set by user
    *
    * @details
    * used for optimiziation to not search the whole buffer when it is very long
    * 'holes' may exist. e.g.
    *   setat(1, "help")
    *   setat(5, "info")
    * would set highestValidIndex to 5, but 2, 3 and 4 are empty
    */
    int highestValidIndex;
} text_complete_t;

#ifdef __cplusplus
extern "C" {
#endif

  void text_complete_init(text_complete_t *p);
  int text_complete_setat(text_complete_t *p, int at, const char* value);

  /**
  * @brief given the current input line <value>, returns how many entires start with <value>
  */
  int text_complete_getmatchcount(text_complete_t *p, const char* value);

  /**
  * @brief 
  *  
  * given the current input line <value>
  * match against all completition entries
  * and copy the <match>th match into <target>
  */
  int text_complete_getmatch(text_complete_t *p, const char* value, int match, char* target);

  /**
  * @brief given "s" and the matches "start" and "stop", advances "s" to "st"
  *
  * given the current input line <value>
  * match against all completition entries
  * write common beginnin of all matches to <target>
  */
  void text_complete_advancecommon(text_complete_t *p, const char* value, char* target);

#ifdef __cplusplus
}
#endif

#endif

