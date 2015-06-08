/*!
   @file
   \brief Contains implementations for the event class and event handling functions.
   \author Jari Ronkainen
*/

#include <wheel_core_event.h>

namespace wheel
{
   //! Map an event to a function
   /*!
      Adds an event mapping

      \param  ev     Event data to search for
      \param  ident  Name of this event, used for debugging / removing single events
      \param  func   Function to call in case of specified event.
   */
   void EventMapping::map_event(const wheel::Event& ev, const wheel::string& ident, std::function<void(wheel::Event& e)> func)
   {
      buffer_t& evd = (buffer_t&)ev.data;
      evd.seek(0);
      uint8_t ev_type = evd.read<uint8_t>();

      if (ev_type == WHEEL_EVENT_TIMER)
      {
         uint64_t ptr = evd.read<uint64_t>();
         ev_timers.push_back((Timer*)ptr);
      }
      else if (ev_type == WHEEL_EVENT_VAR_CHANGED)
      {
         var_tracker_t var_tracker;

         uint64_t ptr = evd.read<uint64_t>();

         var_tracker.ptr = (void*)evd.read<uint64_t>();
         var_tracker.data_size = evd.read<uint64_t>();

         // TODO: FIXME: Calculate hash.
         var_tracker.hash = 0x70d0;
         ev_vars.push_back(var_tracker);
      }

      wheel::eventinfo_t nei;
      nei.ident = ident;
      nei.func = func;

      map_data[ev.data] = nei;
   }

   //! Unmap an event
   /**
    * Remove an event mapping
    * 
    * \param ident   Name of the event to be removed
    */
   void EventMapping::unmap_event(const wheel::string& ident)
   {
      for (auto it = map_data.begin(); it != map_data.end(); ++it)
      {
         if (it->second.ident == ident)
         {
            map_data.erase(it);
            return;
         }
      }
   }

   void EventMapping::process(wheel::EventList& events)
   {
      // Handle timers
      std::list< std::list<wheel::Timer*>::iterator > erase_list;

      for (auto it = ev_timers.begin(); it != ev_timers.end(); ++it)
      {
         if ((*it)->Check())
         {
            if ((*it)->repeat == false)
               erase_list.push_back(it);

            wheel::Event newevent;
            newevent.data.push_back(WHEEL_EVENT_TIMER);

            uint64_t ptr_val = (uint64_t) (*it);
            newevent.data.write<uint64_t>(ptr_val);

            events.push_back(newevent);
         }
      }
      for (auto it : erase_list)
         ev_timers.erase(it);

      // Handle variables
      for (auto var : ev_vars)
      {
         
      }


      if (!active)
         return;

      // Trigger events
      for (wheel::Event e : events)
         for (auto ev : map_data)
         {
            if (match_events(ev.first, e.data))
               ev.second.func(e);
         }
   }

   //! Check events for a match
   /*!
      Compare two event buffers for a match
   */
   uint32_t match_events(const wheel::buffer_t& l, const wheel::buffer_t& r)
   {
      if ((l.size() == 0) || (r.size() == 0))
         return 0;

      size_t i = 0;
      while(i < l.size() && i < r.size())
      {
         if (!((l[i] == r[i]) || (l[i] == WHEEL_ANY) || (r[i] == WHEEL_ANY)))
            return 0;
         ++i;
      }
      return 1;
   }

   uint32_t match_events(const wheel::Event& le, const wheel::Event& re)
   {
      const buffer_t& l = le.data;
      const buffer_t& r = re.data;

      if ((l.size() == 0) || (r.size() == 0))
         return 0;

      size_t i = 0;
      while(i < l.size() && i < r.size())
      {
         if (!((l[i] == r[i]) || (l[i] == WHEEL_ANY) || (r[i] == WHEEL_ANY)))
            return 0;
         ++i;
      }
      return 1;
   }

}