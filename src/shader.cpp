#include <wheel_video.h>

namespace wheel
{
   uint32_t Shader::SetVertexSource(const wcl::string& vs)
   {
      return WHEEL_OK;
   }

   uint32_t Shader::SetFragmentSource(const wcl::string& fs)
   {
      return WHEEL_OK;
   }

   uint32_t Shader::Compile()
   {
   }
   uint32_t Shader::Use()
   {
      return WHEEL_OK;
   }
}
