//Costum extension by woegster, not part of the original ntshell
#include "text_complete.h"
#include "ntlibc.h"

void text_complete_init(text_complete_t *p)
{
  int i;
  for (i = 0; i < (int)sizeof(p->complete); i++)
  {
    p->complete[i] = 0;
  }
  p->highestValidIndex = -1;
}

int text_complete_setat(text_complete_t *p, int at, const char* value)
{
  if (at < 0)
  {
    return -1;
  }

  if (at >= TEXTCOMPLE_MAXENTRIES)
  {
    return -2;
  }

  if (ntlibc_strlen(value) > TEXTCOMPLETE_MAXLEN)
  {
    return -3;
  }  

  ntlibc_strcpy(&p->complete[at * TEXTCOMPLETE_MAXLEN], value);
  if (at > p->highestValidIndex)
  {
    p->highestValidIndex = at;
  }
  return 0;
}

int min(int a, int b)
{
  return a < b ? a : b;
}

//implements "starts with"
int matches_filter(const char* valueToTest, const char* filter)
{
  //value to check does not exist
  int value_len = ntlibc_strlen(valueToTest);
  if (value_len == 0)
  {
    return 0;
  }

  //nothing to check against: matches
  int filter_len = ntlibc_strlen(filter);
  if (filter_len == 0)
  {
    return 1;
  }

  //filter longer then the value: can never match
  if (filter_len > value_len)
  {
    return 0;
  }  
  
  int smallest_len = min(value_len, filter_len);

  int i;
  for (i = 0; i < smallest_len; ++i)
  {
    if (valueToTest[i] != filter[i])
    {
      return 0;
    }
  }

  return 1;
}

int text_complete_getmatchcount(text_complete_t *p, const char* value)
{
  int match_count = 0;

  int i;
  for (i = 0; i <= p->highestValidIndex; i++)
  {
    if (matches_filter(&p->complete[i*TEXTCOMPLETE_MAXLEN], value) == 1)
    {
      match_count = match_count + 1;
    }
  }

  return match_count;
}


int text_complete_getmatch(text_complete_t *p, const char* value, int match, char* target)
{
  int match_id = 0;
  int i;
  for (i = 0; i <= p->highestValidIndex; i++)
  {
    if (matches_filter(&p->complete[i*TEXTCOMPLETE_MAXLEN], value) == 1)
    {
      if (match_id == match)
      {
        ntlibc_strcpy(target, &(p->complete[i*TEXTCOMPLETE_MAXLEN]));
        return 1;
      }
      match_id = match_id + 1;      
    }
  }

  return 0;
}

void text_complete_advancecommon(text_complete_t *p, const char* value, char* target)
{
  ntlibc_strcpy(target, value);

  //for each char in target
  int commonUntil = 0;
  int done = 0;
  while (!done)
  {
    //compare against all candidates
    int i;
    for (i = 0; i <= p->highestValidIndex; i++)
    {
      const char* candidate = &p->complete[i*TEXTCOMPLETE_MAXLEN];
      if (ntlibc_strlen(candidate) > commonUntil)
      {
        if (matches_filter(candidate, value) == 1)
        {
          //if its a match and the char is equal
          if (target[commonUntil] == 0 || target[commonUntil] == candidate[commonUntil])
          {
            target[commonUntil] = candidate[commonUntil];
          }
          else
          {
            done = 1;
          }
        }
      }      
    }

    if (done != 1)
    {
      commonUntil++;
    }
  }

  target[commonUntil] = 0;
}