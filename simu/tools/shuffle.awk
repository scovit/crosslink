# GAWK script to shuffle a file

function shuffle() {
  # To shuffle the array vector of n elements (indexes 0..n-1):
  i = 0;
  j = 0;
  c = 0;

  for (i = n - 1; i > 0; i--) {
    j = int(rand()*32000) % (i + 1);
    c = vector[j];
    vector[j] = vector[i];
    vector[i] = c;
  }
}

BEGIN {
  srand(systime() + PROCINFO["pid"]);
  n = 0;
}

{
  vector[n] = $1;
  n++;
}

END {
  shuffle();
  for (i = 0; i < n; i++) {
    print vector[i];
  }
}
