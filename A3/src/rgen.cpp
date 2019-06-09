#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <math.h>

// to see, s1 == s2 ? or not
bool str_eq(const char *s1, const char *s2) {
  return strcmp(s1, s2) == 0;
}
/**
 * gen random number from lower to upper, both inclusive
 * @param lower
 * @param upper
 * @return
 */
int gen_random(int lower, int upper);

/**
 * check length between (x1, y1)->(x2, y2) and (x3, y3)->(x4, y4)
 * @return 0: longer, 1: shorter, 2: equal
 */
int check_length(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

/**
 * (x1, y1)->(x2, y2) collineation with
 * (x2, y2)->(x3, y3)
 */
int check_collineation(int x1, int y1, int x2, int y2, int x3, int y3);

/**
 * (x3, y3) is in the middle of (x1, y1)->(x2, y2)
 */
int check_between(int x1, int y1, int x2, int y2, int x3, int y3);

int same_point(int x1, int y1, int x2, int y2) {
  return x1 == x2 && y1 == y2;
}

/**
 * (x1, y1)->(x2, y2) overlapped with
 * (x3, y3)->(x4, y4)
 */
int check_overlap(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4);

int gen_random(int lower, int upper) {
  unsigned int random;
  int random_fd = -1;
  if ((random_fd = open("/dev/urandom", O_RDONLY)) < 0) {
    fprintf(stderr, "Error: open /dev/urandom error\n");
    exit(1);
  }
  //  from 0 to (2**31)-1
  read(random_fd, &random, sizeof(unsigned int));
  random %= (upper - lower + 1);
  random += lower;
  close(random_fd);
  return random;
}

int check_length(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4) {
  if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) > sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4))) {
    return 0; // longer
  } else if (sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2)) < sqrt((x3 - x4) * (x3 - x4) + (y3 - y4) * (y3 - y4))) {
    return 1; // shorter
  } else {
    return 2; // equal
  }
}

int check_collineation(int x1, int y1, int x2, int y2, int x3, int y3) {
  if (y1 == y2) {
    if (y3 == y1) {
      return 1;
    } else {
      return 0;
    }
  } else if (x1 == x2) {
    if (x3 == x1) {
      return 1;
    } else {
      return 0;
    }
  } else {

    if ((y2 - y1) * (x3 - x2) == (x2 - x1) * (y3 - y2)) {
      return 1;
    } else {
      return 0;
    }

  }

}

int check_between(int x1, int y1, int x2, int y2, int x3, int y3) {
  if (x1 == x2) {
    if ((y3 - y1) * (y3 - y2) < 0) {
      return 1;
    } else {
      return 0;
    }
  } else {
    if ((x3 - x1) * (x3 - x2) < 0) {
      return 1;
    } else {
      return 0;
    }
  }

}

int check_overlap(int x1, int y1, int x2, int y2, /* segment 1 */
                  int x3, int y3, int x4, int y4  /* segment 2 */  ) {
  if (check_collineation(x1, y1, x2, y2, x3, y3) == 1) {
    if (check_collineation(x1, y1, x2, y2, x4, y4) == 1) {
      if (check_length(x1, y1, x2, y2, x3, y3, x4, y4) == 0) {/* (x1,y1)->(x2,y2) is longer*/
        if (check_between(x1, y1, x2, y2, x3, y3) == 1) {/* stricted between */
          return 1;/* must overlap */
        } else {
          if (check_between(x1, y1, x2, y2, x4, y4) == 1) {/* stricted between */
            return 1;
          } else {
            return 0;
          }
        }
      } else if (check_length(x1, y1, x2, y2, x3, y3, x4, y4) == 1) {/* (x3,y3)->(x4,y4) is longer*/
        if (check_between(x3, y3, x4, y4, x1, y1) == 1) {/* stricted between */
          return 1;/* must overlap */
        } else {
          if (check_between(x3, y3, x4, y4, x2, y2) == 1) {/* stricted between */
            return 1;
          } else {
            return 0;
          }
        }
      } else {/* same length */
        if ((x1 == x3 && y1 == y3 && x2 == x4 && y2 == y4) || (x1 == x4 && x2 == x3 && y1 == y4 && y2 == y3)) {/* four points are entirely the same */
          return 1;
        } else {
          if (check_between(x1, y1, x2, y2, x3, y3) == 1) {/* stricted between */
            return 1;/* must overlap */
          } else {
            if (check_between(x1, y1, x2, y2, x4, y4) == 1) {/* stricted between */
              return 1;
            } else {
              return 0;
            }
          }
        }
      }
    } else {
      return 0;
    }
  } else {
    return 0;
  }

}

void handle_input(
    int &flag_s,
    int &flag_n,
    int &flag_l,
    int &flag_c,
    int &s,
    int &n,
    int &l,
    int &c,
    int argc, char *argv[]) {
  int i; /*loop counter*/
  if (argc == 1) { // no cli args, all using default values
    flag_s = 1;
    s = 10;   // number of streets
    flag_n = 1;
    n = 5;    //  number of line-segments
    flag_l = 1;
    l = 5;    // number of seconds to wait, interval
    flag_c = 1;
    c = 20;   // coordination upper and lower bounds
  } else {
    for (i = 1; i < argc; i += 2) {
      if (str_eq(argv[i], "-s")) {
        flag_s = 1;
        s = atoi(argv[i + 1]);
        if (s < 10) {
          flag_s = 1;
        }
      } else if (str_eq(argv[i], "-n")) {
        flag_n = 1;
        n = atoi(argv[i + 1]);
        if (n < 5) {
          flag_n = 1;
        }
      } else if (str_eq(argv[i], "-l")) {
        flag_l = 1;
        l = atoi(argv[i + 1]);
        if (l < 5) {
          flag_l = 1;
        }
      } else if (str_eq(argv[i], "-c")) {
        flag_c = 1;
        c = atoi(argv[i + 1]);
        if (c < 20) {
          flag_c = 1;
        }
      }
    }
  }
  if (flag_s == 0) {
    flag_s = 1;
    s = 10;
  }
  if (flag_n == 0) {
    flag_n = 1;
    n = 5;
  }
  if (flag_l == 0) {
    flag_l = 1;
    l = 5;
  }
  if (flag_c == 0) {
    flag_c = 1;
    c = 20;
  }
}

int main(int argc, char *argv[]) {

  setvbuf(stdout, NULL, _IONBF, 0);

  int flag_s = 0, flag_n = 0, flag_l = 0, flag_c = 0; // indicate that if provide input from argc/argv or not?
  int s = 0, n = 0, l = 0, c = 0; /* upper bounds of random integers */
  handle_input(flag_s, flag_n, flag_l, flag_c,
               s, n, l, c,
               argc, argv);

  int st, ti;// random  for street, time interval
  int it_st, it_point;/* iterator for street and point/vertex in each street */
  int st_name_id; // street name
  while (true) {
    st = gen_random(2, s);/* number of streets */
    ti = gen_random(5, l);/* time interval */

    char stName[st][10]; // street name can't exceed 10 chars
    int li[st]; // number of line segments for each street
    int point[st][10000]; // vertex quantity can't exceed 10000 for each street

    /* set name for each street */
    for (it_st = 0; it_st < st; it_st++) {
      st_name_id = it_st;
      sprintf(stName[it_st], "st%d", st_name_id); // prepare for street names
    }

    /* generate each point for each street*/
    for (it_st = 0; it_st < st; it_st++) { /* for each street */
      li[it_st] = gen_random(1, n);/* number of line segments of current street */

      for (it_point = 0; it_point <= 2 * li[it_st]; it_point += 2) { /* generate each point for each street */
        point[it_st][it_point] = gen_random(-c, c); // generate x
        point[it_st][it_point + 1] = gen_random(-c, c); // generate y

        /* first point of first street, no need to check errors*/
        if (it_st == 0 && it_point == 0)
          continue;

          /* check errors */
          /*check if generate same point and overlap*/
        else if (it_st == 0 && it_point != 0) {
          if (it_point == 2) {/* just check same point within the current street */

            int count = 1;

            while (same_point(point[it_st][0], point[it_st][1],/*previous point, which is the very first point*/
                              point[it_st][it_point], point[it_st][it_point + 1]) /*current point*/) {
              count++;
              if (count > 25) {
                fprintf(stderr, "Error: failed to generate valid input for 25 simultaneous attempts\n");
                exit(3);
              }
              point[it_st][it_point] = gen_random(-c, c);
              point[it_st][it_point + 1] = gen_random(-c, c);
            }
          } else if (it_point > 2) {/* use four nodes to check overlap */
            /* check overlap and same point */
            int t;
            for (t = 0; t < it_point - 2; t += 2) {
              int count3 = 1;
              while ((
                  // overlap
                  check_overlap(
                      // for each line segment
                      point[it_st][t], point[it_st][t + 1], // (x1,y1)
                      point[it_st][t + 2], point[it_st][t + 3],  // (x2,y2)
                      // current line segment
                      point[it_st][it_point - 2], point[it_st][it_point - 1], // (x3,y3)
                      point[it_st][it_point], point[it_st][it_point + 1] /* (x4,y4) */ ) == 1) ||
                  // or same point
                  same_point(point[it_st][it_point - 2], point[it_st][it_point - 1], /* previous point */
                             point[it_st][it_point], point[it_st][it_point + 1] /* current point */)) {
                count3++;
                if (count3 > 25) {
                  fprintf(stderr, "Error: failed to generate valid input for 25 simultaneous attempts\n");
                  exit(3);
                }
                point[it_st][it_point] = gen_random(-c, c);
                point[it_st][it_point + 1] = gen_random(-c, c);
              }
            }
          }

        } else if (it_st != 0) { // if not first street
          if (it_point >= 2) {/* use four nodes to check overlap */
            int valid = 0;
            int count1 = 1;
            while (valid == 0) {
              int flag = 0;
              /* 1. check same point first */
              if (same_point(point[it_st][it_point - 2], point[it_st][it_point - 1], /* previous point */
                             point[it_st][it_point], point[it_st][it_point + 1] /* current point */)) {
                if (count1 > 25) {
                  fprintf(stderr, "Error: failed to generate valid input for 25 simultaneous attempts\n");
                  exit(3);
                }
                count1++;
                point[it_st][it_point] = gen_random(-c, c);
                point[it_st][it_point + 1] = gen_random(-c, c);
                continue;
              }

              int t1, t2;
              /* 2. check overlap between current line seg and other generated streets */
              for (t1 = 0; t1 < it_st; t1++) {
                for (t2 = 0; t2 < 2 * li[t1]; t2 += 2) {
                  if (check_overlap(
                      // for each segment in street t1
                      point[t1][t2], point[t1][t2 + 1],
                      point[t1][t2 + 2], point[t1][t2 + 3],
                      // current segment
                      point[it_st][it_point - 2], point[it_st][it_point - 1],
                      point[it_st][it_point], point[it_st][it_point + 1]) == 1) {

                    flag = 1;
                    break;
                  }

                }
                if (flag == 1) {
                  break;
                }
              }
              if (flag == 1) {
                if (count1 > 25) {
                  fprintf(stderr, "Error: failed to generate valid input for 25 simultaneous attempts\n");
                  exit(3);
                }
                count1++;
                point[it_st][it_point] = gen_random(-c, c);
                point[it_st][it_point + 1] = gen_random(-c, c);
                continue;
              }

              /* 3. check overlap within current street */
              for (t1 = 0; t1 < it_point - 2; t1 += 2) {
                if (check_overlap(
                    // each segment on current street
                    point[it_st][t1], point[it_st][t1 + 1],
                    point[it_st][t1 + 2], point[it_st][t1 + 3],
                    // current segment
                    point[it_st][it_point - 2], point[it_st][it_point - 1],
                    point[it_st][it_point], point[it_st][it_point + 1]) == 1) {

                  flag = 1;
                  break;
                }
              }

              if (flag == 1) {
                if (count1 > 25) {
                  fprintf(stderr, "Error: failed to generate valid input for 25 simultaneous attempts\n");
                  exit(3);
                }
                count1++;
                point[it_st][it_point] = gen_random(-c, c);
                point[it_st][it_point + 1] = gen_random(-c, c);
                continue;
              }

              valid = 1;

            }//end while(valid == 0)
          }//end if, it_point >=2
        }//end else if it_st != 0, i.e, not the 1st street
      }//end for it_point, for each point
    }//end for it_st, for each street

    /* send spec to a1ece650.py */
    for (it_st = 0; it_st < st; it_st++) {
      printf("a \"%s\" ", stName[it_st]);
      for (it_point = 0; it_point <= 2 * li[it_st]; it_point += 2) {
        printf("(%d,%d) ", point[it_st][it_point], point[it_st][it_point + 1]);
      }
      printf("\n");
    }
    printf("g\n");
    sleep(ti);
    for (it_st = 0; it_st < st; it_st++) { // remove old streets
      printf("r \"%s\" \n", stName[it_st]);
    }
  }//end while

  return 0;
}

