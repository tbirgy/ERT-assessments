set terminal gif animate delay 5
set output 'square_wave_transport.gif'
set xlabel 'Position (m)'
set ylabel 'Density'
set yrange [0:26]
set xrange [0:1000]

do for [i=0:199] {
    set title sprintf('Time step: %d sec', i/20.0)
    plot sprintf('frame_%03d.dat', i) with lines title ''
}

# System command to delete the frame files
system('del frame_*.dat')
