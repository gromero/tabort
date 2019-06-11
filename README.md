# Prepare system

## On Ubuntu

```
$ echo "deb http://ddebs.ubuntu.com $(lsb_release -cs) main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/ddebs.list
$ echo "deb http://ddebs.ubuntu.com $(lsb_release -cs)-updates main restricted universe multiverse" | sudo tee -a /etc/apt/sources.list.d/ddebs.list
$ sudo apt-key adv --keyserver keyserver.ubuntu.com --recv-keys C8CAB6595FDFF622
$ sudo apt-get update
$ sudo apt-get install linux-image-$(uname -r)-dbgsym
$ sudo apt-get install systemtap
$ sudo stap-prep
```

## On RHEL

```
git clone https://github.com/gromero/tabort.git && cd tabort
sudo yum install systemtap -y
yum install kernel-devel-$(uname -r) -y
sudo debuginfo-install kernel-$(uname -r) -y
sudo stap -v ./tm.d
```

# Run probe

```
$ git clone git@github.com:gromero/tabort.git && cd tabort
$ sudo stap -v ./tm.d 
WARNING: Kernel function symbol table missing [man warning::symbols]
Pass 1: parsed user script and 488 library scripts using 63744virt/47808res/8512shr/40832data kb, in 270usr/10sys/288real ms.
Pass 2: analyzed script: 1 probe, 1 function, 3 embeds, 0 globals using 118336virt/107328res/10240shr/95424data kb, in 2270usr/420sys/5299real ms.
Pass 3: translated to C into "/tmp/stap4ilLpx/stap_9e85e61cefea1055ecdaf881a22cd075_2090_src.c" using 118336virt/107584res/10496shr/95424data kb, in 10usr/0sys/7real ms.
Pass 4: compiled C into "stap_9e85e61cefea1055ecdaf881a22cd075_2090.ko" in 11550usr/670sys/12562real ms.
Pass 5: starting run.
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> extract_opal_du
tm_unavailable() -> qemu-system-ppc
tm_unavailable() -> extract_opal_du
tm_unavailable() -> CPU 2/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> extract_opal_du
tm_unavailable() -> extract_opal_du
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> extract_opal_du
tm_unavailable() -> extract_opal_du
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> CPU 1/KVM
tm_unavailable() -> extract_opal_du
tm_unavailable() -> extract_opal_du
...
 
```

If system has not `tabort` before `sc` (i.e. it is *NOT* using TM by any means) after `Pass 5:` there will be no output, i.e. no `tm_unavailable()` will be callled on an idle system.
