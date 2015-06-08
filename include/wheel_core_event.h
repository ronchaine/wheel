#ifndef WHEEL_CORE_EVENT_HEADER
#define WHEEL_CORE_EVENT_HEADER

#include "wheel_core_string.h"

#include <unordered_map>

namespace wheel
{
   //! Event
   /*!
   */
   class Event
   {
      public:
         buffer_t    data;

         string      get_event_string() const;
   };

   typedef std::list<Event> EventList;

   struct eventinfo_t
   {
      wcl::string                            ident;
      std::function<void(wheel::Event& e)>   func;
   };

   // Move to .cpp?
   typedef std::unordered_map<wheel::buffer_t, eventinfo_t> eventlinks_t;

   struct event_map_t
   {
      bool           active;
      wheel::string  id;
      eventlinks_t   map;
   };

   template<typename ...Args>
   inline wheel::Event describe_event(Args ... codes)
   {
      wheel::Event rval;
      uint8_t args[] { (uint8_t)codes ... };

      for (uint8_t c : args)
      {
         rval.data.push_back(c);
      }

      return rval;
   }

   template<typename T>
   inline wheel::Event variable_event(T& var)
   {
      wheel::Event rval;
      rval.data.seek(0);
      rval.data.write<uint8_t>(WHEEL_EVENT_VAR_CHANGED);
      rval.data.write<uint64_t>(&var);

      uint8_t* var_byte_ptr = (uint8_t*)&var;
      for (int i = 0; i < sizeof(var); ++i)
      {
         rval.data.write<uint8_t>(*(var_byte_ptr + i));
      }
   }

   class EventMapping
   {
      private:
         eventlinks_t   map_data;
         bool           active;

      public:
         wheel::string  id;

         bool           is_active() const;
         void           map_event(const wheel::Event&, const wheel::string& ident, std::function<void(wheel::Event&)>);
         void           unmap_event(const wheel::string& ident);
   };

   uint32_t match_events(const wheel::buffer_t& l, const wheel::buffer_t& r);
   uint32_t match_events(const wheel::Event& l, const wheel::Event& r);
}

#endif