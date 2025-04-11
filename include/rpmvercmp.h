#pragma once
#include <cstring>

static inline int Rislower(int c) {
    return (c >= 'a' && c <= 'z');
}

static inline int Risupper(int c) {
    return (c >= 'A' && c <= 'Z');
}

static inline int Risalpha(int c) {
    return (Rislower(c) || Risupper(c));
}

static inline int Risdigit(int c) {
    return (c >= '0' && c <= '9');
}
static inline int Rstreq(const char *s1, const char *s2) {
    return (strcmp(s1, s2) == 0);
}

static inline int Risalnum(int c) {
    return (Risalpha(c) || Risdigit(c));
}

static int Rpmvercmp(const char *a, const char *b) {
    /* easy comparison to see if versions are identical */
    if (Rstreq(a, b)) {
        return 0;
    }

    char oldch1, oldch2;
    char abuf[strlen(a) + 1], bbuf[strlen(b) + 1];
    char *str1 = abuf, *str2 = bbuf;
    char *one, *two;
    int rc;
    int isnum;

    strcpy(str1, a);
    strcpy(str2, b);

    one = str1;
    two = str2;

    /* loop through each version segment of str1 and str2 and compare them */
    while (*one || *two) {
        while (*one && !Risalnum(*one) && *one != '~') {
            one++;
        }
        while (*two && !Risalnum(*two) && *two != '~') {
            two++;
        }

        /* handle the tilde separator, it sorts before everything else */
        if (*one == '~' || *two == '~') {
            if (*one != '~') {
                return 1;
            }
            if (*two != '~') {
                return -1;
            }
            one++;
            two++;
            continue;
        }

        /* If we ran to the end of either, we are finished with the loop */
        if (!(*one && *two)) {
            break;
        }

        str1 = one;
        str2 = two;

        /* grab first completely alpha or completely numeric segment */
        /* leave one and two pointing to the start of the alpha or numeric */
        /* segment and walk str1 and str2 to end of segment */
        if (Risdigit(*str1)) {
            while (*str1 && Risdigit(*str1)) {
                str1++;
            }
            while (*str2 && Risdigit(*str2)) {
                str2++;
            }
            isnum = 1;
        } else {
            while (*str1 && Risalpha(*str1)) {
                str1++;
            }
            while (*str2 && Risalpha(*str2)) {
                str2++;
            }
            isnum = 0;
        }

        /* save character at the end of the alpha or numeric segment */
        /* so that they can be restored after the comparison */
        oldch1 = *str1;
        *str1 = '\0';
        oldch2 = *str2;
        *str2 = '\0';

        /* this cannot happen, as we previously tested to make sure that */
        /* the first string has a non-null segment */
        if (one == str1) {
            return -1; /* arbitrary */
        }

        /* take care of the case where the two version segments are */
        /* different types: one numeric, the other alpha (i.e. empty) */
        /* numeric segments are always newer than alpha segments */
        /* XXX See patch #60884 (and details) from bugzilla #50977. */
        if (two == str2) {
            return (isnum ? 1 : -1);
        }

        if (isnum) {
            size_t onelen, twolen;
            /* this used to be done by converting the digit segments */
            /* to ints using atoi() - it's changed because long  */
            /* digit segments can overflow an int - this should fix that. */

            /* throw away any leading zeros - it's a number, right? */
            while (*one == '0') {
                one++;
            }
            while (*two == '0') {
                two++;
            }

            /* whichever number has more digits wins */
            onelen = strlen(one);
            twolen = strlen(two);
            if (onelen > twolen) {
                return 1;
            }
            if (twolen > onelen) {
                return -1;
            }
        }

        /* strcmp will return which one is greater - even if the two */
        /* segments are alpha or if they are numeric.  don't return  */
        /* if they are equal because there might be more segments to */
        /* compare */
        rc = strcmp(one, two);
        if (rc) {
            return (rc < 1 ? -1 : 1);
        }

        /* restore character that was replaced by null above */
        *str1 = oldch1;
        one = str1;
        *str2 = oldch2;
        two = str2;
    }

    /* this catches the case where all numeric and alpha segments have */
    /* compared identically but the segment sepparating characters were */
    /* different */
    if ((!*one) && (!*two)) {
        return 0;
    }

    /* whichever version still has characters left over wins */
    if (!*one) {
        return -1;
    } else {
        return 1;
    }
}