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

      \param  mapid  Name of the map to insert the mapping
      \param  ev     Event data to search for
      \param  ident  Name of this event, used for debugging / removing single events
      \param  func   Function to call in case of specified event.
   */
   void EventMapping::map_event(const wheel::Event& ev, const wheel::string& ident, std::function<void(wheel::Event& e)> func)
   {
      wheel::eventinfo_t nei;
      nei.ident = ident;
      nei.func = func;

      map_data[ev.data] = nei;
   }

}