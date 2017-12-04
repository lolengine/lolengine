
Generate vcxproj for Bullet:

    cat lol-bullet.am | \
        awk '/bullet_source/ { print "  <ItemGroup>" } \
             /NULL/ { print "  </ItemGroup>" } \
             /bullet3.*\.(c|cpp)\>/ { print "    <ClCompile Include=\""$1"\" />" } \
             /bullet3.*\.(h|hpp|cl)\>/ { print "    <ClInclude Include=\""$1"\" />" }'

