// $Id: listmap.tcc,v 1.15 2019-10-30 12:44:53-07 - - $

#include "listmap.h"
#include "debug.h"

//
/////////////////////////////////////////////////////////////////
// Operations on listmap.
/////////////////////////////////////////////////////////////////
//

//
// listmap::~listmap()
//
template <typename key_t, typename mapped_t, class less_t>
listmap<key_t,mapped_t,less_t>::~listmap() {
   DEBUGF ('l', reinterpret_cast<const void*> (this));

   iterator begin = this->begin();

   while (not this->empty()) {
      begin = erase(begin);
   }
}

//
// iterator listmap::insert (const value_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::insert (const value_type& pair) {
   DEBUGF ('l', &pair << "->" << pair);

   if (this->empty()) { // If listmap is empty
      node* anchor = this->anchor();
      node* new_node = new node(anchor, anchor, pair);

      anchor->prev = new_node;
      anchor->next = new_node;
   }
   else { // Else if listmap is not empty
      key_type key = pair.first;

      iterator found_iterator = find(key);
      iterator end = this->end();

      if (found_iterator == end) { // If same key was not found
         iterator begin = this->begin();

         for (auto it = begin; it != end; ++it) {
            key_type other_key = it->first;

            if (not less(other_key, key)) {
               node* current_node = it.where;
               node* prev_node = current_node->prev;

               node* new_node = new node(current_node, prev_node, pair);

               prev_node->next = new_node;
               current_node->prev = new_node;

               return --it;
            }
         }

         // If code got to here, it means iterator came to end and must insert as last node
         node* end_node = end.where;
         node* end_node_prev = end_node->prev;

         node* new_node = new node(end_node, end_node_prev, pair);

         end_node_prev->next = new_node;
         end_node->prev = new_node;

         return iterator(new_node);
      }
      else { // Else if same key was found
         mapped_type value = pair.second;

         found_iterator->second = value; 

         return found_iterator;
      }
   }

   return iterator(); // Should never get to here, but need this to stop "warning: control reaches end of non-void function"
}

//
// listmap::find(const key_type&)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::find (const key_type& that) {
   DEBUGF ('l', that);

   iterator begin = this->begin();
   iterator end = this->end();

   for (auto it = begin; it != end; ++it) {
      key_type other_key = it->first;

      if (not less(that, other_key) && not less(other_key, that)) {
         return it;
      }
   }

   // If code got to here, it means iterator came to end and must return end iterator
   return end;
}

//
// iterator listmap::erase (iterator position)
//
template <typename key_t, typename mapped_t, class less_t>
typename listmap<key_t,mapped_t,less_t>::iterator
listmap<key_t,mapped_t,less_t>::erase (iterator position) {
   DEBUGF ('l', &*position);

   node* current_node = position.where;
   node* prev_node = current_node->prev;
   node* next_node = current_node->next;

   prev_node->next = next_node;
   next_node->prev = prev_node;

   iterator next_iterator = ++position;

   delete current_node;

   return next_iterator;
}


