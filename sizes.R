library(dplyr)

d.mac    <- read.table("mac-sizes.txt");
d.ubuntu <- read.table("ubuntu-sizes.txt");
names(d.mac) <- c("size", "product", "version", "group")
names(d.ubuntu) <- c("size", "product", "version", "group")
d.mac$size = d.mac$size/1024.0
d.ubuntu$size = d.ubuntu$size/1024.0

sum.mac = d.mac %>% group_by(group) %>% summarise(sz=sum(size))
sum.ubuntu = d.ubuntu %>% group_by(group) %>% summarise(sz=sum(size))
write.table(sum.mac, file="mac.sizes.txt", row.names=FALSE)
write.table(sum.ubuntu, file="ubuntu.sizes.txt", row.names=FALSE)


