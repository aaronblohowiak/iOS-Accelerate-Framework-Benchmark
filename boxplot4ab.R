
## here is one demo, where type of data is reflected by indicator 'type'
## make some fake data for me:

data <- data.frame(type= c( rep("A",100), rep("B",200)),
                   time= rchisq( 300, c( rep(2,100), rep(1,100) )))
b <- boxplot( data$time ~ as.factor(data$type),
        ylab="Time", frame=FALSE)

## your data is
dim(d2 <- read.csv("output.csv",header=FALSE))
names(d2) <- c("Naive","SIMD")

## look at data: in nanoseconds
t(apply( d2, 2, summary))

## let's put it into better format for pretty/easy plotting
d3 <- data.frame(type=c(rep("Naive",nrow(d2)), rep("SIMD",nrow(d2))),
                 time=c(d2[,1],d2[,2]))

pdf("box-basic.pdf",4,4)
## basic, but ugly wiht different scales
boxplot( I(d3$time/1000000)  ~ as.factor(d3$type),
        ylab="milliseconds", frame=FALSE)

## log scale doesn't help... :(
## this is still ugly, but a first cut, with nicer labels, and exports pdf
pdf("box.pdf",4,4)
boxplot( I(d3$time/1000000) ~ as.factor(d3$type),
        ylab="milliseconds", axes=FALSE, log="y")
axis(1, at=c(1), labels=c("Naive"),tick=FALSE)
axis(1, at=c(2), labels=c("SIMD"),tick=FALSE)

# axis(2, at=c(0.0, 0.40,0.80,0.160,seq(300,800,100)))
dev.off()


## split into 2 plot
# dd <- subset( d3, type=="Naive")
# boxplot( I(dd$time/1000000) ,  ylab="milliseconds", frame=FALSE, log="y", ylim=c(40,800))



