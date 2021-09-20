IncludeDir = {}
IncludeDir['SDL'] = "%{wks.location}/Aura/Dependencies/SDL/include"
IncludeDir['ImGui'] = "%{wks.location}/Aura/Dependencies/imgui"
IncludeDir['Glad'] = "%{wks.location}/Aura/Dependencies/Glad/include"

LibraryDir = {}
LibraryDir["SDL"] = "%{wks.location}/Aura/Dependencies/SDL/lib/x64/"

Library = {}
Library["SDL"] = "%{wks.location}/Aura/Dependencies/SDL/lib/x64/SDL2.lib"
