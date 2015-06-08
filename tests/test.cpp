#include <wheel.h>

int main(void)
{
   wheel::EventMapping events;

   wheel::Timer t5("timer", 5000000, true);
   wheel::Timer t2("timer2", 2000000, true);

   printf("created timer at %08x, with 5000 µsec interval\n", &t5);
   printf("created timer at %08x, with 2000 µsec interval\n", &t2);

   events.map_event(wheel::describe_event2(WHEEL_EVENT_TIMER, &t5), "test timer", [&](wheel::Event& e)
      { 
         std::cout << "second!\n";
      }
   );

   events.map_event(wheel::describe_event2(WHEEL_EVENT_TIMER, &t2), "test timer2", [&](wheel::Event& e)
      { 
         std::cout << "faster second!\n";
      }
   );


   while(1)
   {
      events.process();
   }

   return 0;
}
