#ifndef WHEEL_CORE_EVENT_HEADER
#define WHEEL_CORE_EVENT_HEADER

namespace wheel
{
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
}

#endif