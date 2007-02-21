#ifndef _H_utility
#define _H_utility

#include <stdlib.h>
#include <stdio.h>


void Failure(const char *format, ...);


#define Assert(expr)  \
  ((expr) ? (void)0 : Failure("Assertion failed: %s, line %d:\n    %s", __FILE__, __LINE__, #expr))


void PrintDebug(const char *key, const char *format, ...);

void SetDebugForKey(const char *key, bool val);

bool IsDebugOn(const char *key);

void ParseCommandLine(int argc, char *argv[]);
 

#include <deque>
  

template<typename Element> class List {

 private:
    std::deque<Element> elems;

 public:
           // Create a new empty list
    List() {}

           // Returns count of elements currently in list
    int NumElements() const
	{ return elems.size(); }

          // Returns element at index in list. Indexing is 0-based.
          // Raises an assert if index is out of range.
    Element Nth(int index) const
	{ Assert(index >= 0 && index < NumElements());
	  return elems[index]; }

          // Inserts element at index, shuffling over others
          // Raises assert if index out of range
    void InsertAt(const Element &elem, int index)
	{ Assert(index >= 0 && index <= NumElements());
	  elems.insert(elems.begin() + index, elem); }

          // Adds element to list end
    void Append(const Element &elem)
	{ elems.push_back(elem); }

         // Removes element at index, shuffling down others
         // Raises assert if index out of range
    void RemoveAt(int index)
	{ Assert(index >= 0 && index < NumElements());
	  elems.erase(elems.begin() + index); }

    void PrintAll(int indentLevel, const char *label = NULL)
        { for (int i = 0; i < NumElements(); i++)
             Nth(i)->Print(indentLevel, label); }
             
};

#include <map>
#include <string>
using namespace std;
    
struct ltstr {
  bool operator()(const char* s1, const char* s2) const
  { return strcmp(s1, s2) < 0; }
};


template <class Value> class Iterator; 
 
template<class Value> class Hashtable {

  private: 
     multimap<const char*, Value, ltstr> mmap;
 
   public:
            // ctor creates a new empty hashtable
     Hashtable() {}

           // Returns number of entries currently in table
     int NumEntries() const;

           // Associates value with key. If a previous entry for
           // key exists, the bool parameter controls whether 
           // new value overwrites the previous (removing it from
           // from the table entirely) or just shadows it (keeps previous
           // and adds additional entry). The lastmost entered one for an
           // key will be the one returned by Lookup.
     void Enter(const char *key, Value value,
		    bool overwriteInsteadOfShadow = true);

           // Removes a given key->value pair.  Any other values
           // for that key are not affected. If this is the last
           // remaining value for that key, the key is removed
           // entirely.
     void Remove(const char *key, Value value);

          // Returns value stored under key or NULL if no match.
          // If more than one value for key (ie shadow feature was
          // used during Enter), returns the lastmost entered one.
     Value Lookup(const char *key);

          // Returns an Iterator object (see below) that can be used to
          // visit each value in the table in alphabetical order.
     Iterator<Value> GetIterator();

};


/* Don't worry too much about how the Iterator is implemented, see
 * sample usage above for how to iterate over a hashtable using an
 * iterator.
 */
template<class Value> class Iterator {

  friend class Hashtable<Value>;

  private:
    typename multimap<const char*, Value , ltstr>::iterator cur, end;
    Iterator(multimap<const char*, Value, ltstr>& t)
	: cur(t.begin()), end(t.end()) {}
	 
  public:
         // Returns current value and advances iterator to next.
         // Returns NULL when there are no more values in table
         // Visits every value, even those that are shadowed.
    Value GetNextValue();
};

template <class Value> void Hashtable<Value>::Enter(const char *key, Value val, bool overwrite)
{
  Value prev;
  if (overwrite && (prev = Lookup(key)))
    Remove(key, prev);
  mmap.insert(make_pair(strdup(key), val));
}

template <class Value> void Hashtable<Value>::Remove(const char *key, Value val)
{
  if (mmap.count(key) == 0) // no matches at all
    return;

  typename multimap<const char *, Value>::iterator itr;
  itr = mmap.find(key); // start at first occurrence
  while (itr != mmap.upper_bound(key)) {
    if (itr->second == val) { // iterate to find matching pair
	mmap.erase(itr);
	break;
    }
    ++itr;
  }
} 


/* Hashtable::Lookup
 * -----------------
 * Returns the value earlier stored under key or NULL
 *if there is no matching entry
 */
template <class Value> Value Hashtable<Value>::Lookup(const char *key) 
{
  Value found = NULL;
  
  if (mmap.count(key) > 0) {
    typename multimap<const char *, Value>::iterator cur, last, prev;
    cur = mmap.find(key); // start at first occurrence
    last = mmap.upper_bound(key);
    while (cur != last) { // iterate to find last entered
	prev = cur; 
	if (++cur == mmap.upper_bound(key)) { // have to go one too far
	  found = prev->second; // one before last was it
	  break;
	}
    }
  }
  return found;
}


/* Hashtable::NumEntries
 * ---------------------
 */
template <class Value> int Hashtable<Value>::NumEntries() const
{
  return mmap.size();
}



/* Hashtable:GetIterator
 * ---------------------
 * Returns iterator which can be used to walk through all values in table.
 */
template <class Value> Iterator<Value> Hashtable<Value>::GetIterator() 
{
  return Iterator<Value>(mmap);
}


/* Iterator::GetNextValue
 * ----------------------
 * Iterator method used to return current value and advance iterator
 * to next entry. Returns null if no more values exist.
 */
template <class Value> Value Iterator<Value>::GetNextValue()
{
  return (cur == end ? NULL : (*cur++).second);
}

#endif
