## OSM map
OpenStreetMap (OSM) 数据是以 XML 格式存储的。以下是主要元素的简要概述：

- <osm>: XML 文档的根元素。
- <node>: 表示空间中的一个点。每个节点都有一个 id，以及纬度和经度属性。
- <way>: 表示一个折线，或者当它的最后一个节点与第一个节点相同时，表示一个多边形。一个 way 包含对节点的引用。
- <relation>: 表示两个或多个数据元素（节点、路径和其他关系）之间的关系。
- <tag>: 表示附加到节点、路径或关系上的元数据。每个标签都有一个键和一个值

## Linux
Linux stores a lot of system data in files within the /proc directory.
Information about the operating system exists outside of the /proc directory, in the /etc/os-release file.
Information about the kernel exists /proc/version file.
Information about memory utilization exists in the /proc/meminfo file
Information about the total number of processes on the system exists in the /proc/stat file.
Information about the number of processes on the system that are currently running exists in the /proc/meminfo file. 
Information about system up time exists in the /proc/uptime file.
/etc/passwd(opens in a new tab) contains the information necessary to match the UID to a username.
Linux stores memory utilization for the process in /proc/[pid]/status.
Linux stores the process up time in /proc/[pid]/stat.
Linux stores the command used to launch the function in the /proc/[pid]/cmdline file

