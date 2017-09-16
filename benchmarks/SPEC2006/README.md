# SPEC2006

As SPEC2006 is not a free benchmark suit, we can not provide it here in this repository. We provide bellow the commands that we used to execute the benchmarks.



__astar:__
astar BigLakes2048.cfg > BigLakes2048.out 2>> BigLakes2048.err
astar rivers.cfg > rivers.out 2>> rivers.err

__bwaves:__
bwaves 2>> bwaves.err

__bzip2:__
bzip2 input.source 280 > input.source.out 2>> input.source.err
bzip2 chicken.jpg 30 > chicken.jpg.out 2>> chicken.jpg.err
bzip2 liberty.jpg 30 > liberty.jpg.out 2>> liberty.jpg.err
bzip2 input.program 280 > input.program.out 2>> input.program.err
bzip2 text.html 280 > text.html.out 2>> text.html.err
bzip2 input.combined 200 > input.combined.out 2>> input.combined.err

__cactusADM:__
cactusADM benchADM.par > benchADM.out 2>> benchADM.err

__calculix:__
calculix -i  hyperviscoplastic > hyperviscoplastic.log 2>> hyperviscoplastic.err

__dealII:__
dealII 23 > log 2>> dealII.err

__gamess:__
gamess < cytosine.2.config > cytosine.2.out 2>> cytosine.2.err
gamess < h2ocu2+.gradient.config > h2ocu2+.gradient.out 2>> h2ocu2+.gradient.err
gamess < triazolium.config > triazolium.out 2>> triazolium.err

__gcc:__
gcc 166.in -o 166.s > 166.out 2>> 166.err
gcc 200.in -o 200.s > 200.out 2>> 200.err
gcc c-typeck.in -o c-typeck.s > c-typeck.out 2>> c-typeck.err
gcc cp-decl.in -o cp-decl.s > cp-decl.out 2>> cp-decl.err
gcc expr.in -o expr.s > expr.out 2>> expr.err
gcc expr2.in -o expr2.s > expr2.out 2>> expr2.err
gcc g23.in -o g23.s > g23.out 2>> g23.err
gcc s04.in -o s04.s > s04.out 2>> s04.err
gcc scilab.in -o scilab.s > scilab.out 2>> scilab.err

__GemsFDTD:__
GemsFDTD > ref.log 2>> ref.err

__gobmk:__
gobmk --quiet --mode gtp < 13x13.tst > 13x13.out 2>> 13x13.err
gobmk --quiet --mode gtp < nngs.tst > nngs.out 2>> nngs.err
gobmk --quiet --mode gtp < score2.tst > score2.out 2>> score2.err
gobmk --quiet --mode gtp < trevorc.tst > trevorc.out 2>> trevorc.err
gobmk --quiet --mode gtp < trevord.tst > trevord.out 2>> trevord.err

__gromacs:__
gromacs -silent -deffnm gromacs -nice 0 2>> gromacs.err

__h264ref:__
h264ref -d foreman_ref_encoder_baseline.cfg > foreman_ref_baseline_encodelog.out 2>> foreman_ref_baseline_encodelog.err
h264ref -d foreman_ref_encoder_main.cfg > foreman_ref_main_encodelog.out 2>> foreman_ref_main_encodelog.err
h264ref -d sss_encoder_main.cfg > sss_main_encodelog.out 2>> sss_main_encodelog.err

__hmmer:__
hmmer nph3.hmm swiss41 > nph3.out 2>> nph3.err
hmmer --fixed 0 --mean 500 --num 500000 --sd 350 --seed 0 retro.hmm > retro.out 2>> retro.err

__lbm:__
lbm 3000 reference.dat 0 0 100_100_130_ldc.of > lbm.out 2>> lbm.err

__leslie3d:__
leslie3d < leslie3d.in > leslie3d.stdout 2>> leslie3d.err

__libquantum:__
libquantum 1397 8 > ref.out 2>> ref.err

__mcf:__
mcf inp.in > inp.out 2>> inp.err

__milc:__
milc < su3imp.in > su3imp.out 2>> su3imp.err

__namd:__
namd --input namd.input --iterations 38 --output namd.out > namd.stdout 2>> namd.err

__omnetpp:__
omnetpp omnetpp.ini > omnetpp.log 2>> omnetpp.err

__perlbench:__
perlbench -I./lib checkspam.pl 2500 5 25 11 150 1 1 1 1 > checkspam.2500.5.25.11.150.1.1.1.1.out 2>> checkspam.2500.5.25.11.150.1.1.1.1.err
perlbench -I./lib diffmail.pl 4 800 10 17 19 300 > diffmail.4.800.10.17.19.300.out 2>> diffmail.4.800.10.17.19.300.err
perlbench -I./lib splitmail.pl 1600 12 26 16 4500 > splitmail.1600.12.26.16.4500.out 2>> splitmail.1600.12.26.16.4500.err

__povray:__
povray SPEC-benchmark-ref.ini > SPEC-benchmark-ref.stdout 2>> SPEC-benchmark-ref.stderr

__sjeng:__
sjeng ref.txt > ref.out 2>> ref.err

__soplex:__
soplex -s1 -e -m45000 pds-50.mps > pds-50.mps.out 2>> pds-50.mps.stderr
soplex -m3500 ref.mps > ref.out 2>> ref.stderr

__sphinx3:__
sphinx3 ctlfile . args.an4 > an4.log 2>> an4.err

__tonto:__
tonto > tonto.out 2>> tonto.err

__xalancbmk:__
xalancbmk -v t5.xml xalanc.xsl > ref.out 2>> ref.err

__zeusmp:__
zeusmp > zeusmp.stdout 2>> zeusmp.err

