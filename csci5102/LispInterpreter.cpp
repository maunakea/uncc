
/************************************************
 *                                              *
 * Author: Sathish Ramamoorthy                  *
 * Course: CSCI 5102                            *
 * Project 3: Simulation of Lisp Interpreter    *
 * Due Date: 11/29/2000                         *
 *                                              *
 ************************************************/

#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <iostream.h>


char* TRUE  = "T           ";
char* FALSE = "NIL         ";

class Node;
typedef Node* link;

class Item {

public:
	char entry[12];
	Node* value;
	Item* chain;
	Item(char* name, Item* t) {
		strncpy(entry, name, 12);
		value = 0;
		chain = t;
	}
};

typedef Item* point;

class Table {
private:
	point first, last;
	char* F;
	char* T;
public:
	Table() {
//		F = "NIL         ";
//		T = "T           ";
		F = FALSE;
		T = TRUE;
		first = new Item(F, 0);
		last  = new Item(T, first);
	}

	point locate(char* name) {
		point place;
		place = last;
		while (place != NULL) {
			if (strcmp(place->entry, name) == 0)
				return (place);
			place = place->chain;
		}
		place = new Item(name, last);
		last = place;
		return (last);
	}

	void dump() {
		point place;
		cout << endl << "---------------------------------";
		cout << endl << " The list of symbols in the Table" << endl;
		place = last;
		while (place != NULL) {
			cout << endl << "  " << place->entry;
			place = place->chain;
		}
		cout << endl << "---------------------------------" << endl;
	}
};

Table symbols;

enum kind {S, I, L};

class Node {
public:
	kind tag;
	union {
		point a;
		int   b;
		link  c;
	} car;
	link couder;
	Node (point t) {car.a = t; tag = S; couder = 0;}
	Node (int i)   {car.b = i; tag = I; couder = 0;}
	Node (link n)  {car.c = n; tag = L; couder = 0;}
	Node ()        {car.c = 0; tag = L; couder = 0;}
};

void print(link p) {
	link t;
	if (p == 0)
		cout << " print: NIL pointer ";
	else if (p->tag == S)
		cout << " " << p->car.a->entry << " ";
	else if (p->tag == I)
		cout << " " << p->car.b << " ";
	else if (p->tag == L) {
		cout << " (";
		t = p->car.c;
		while (t != 0) {print(t); t = t->couder; }
		cout << ") ";
	}
}

link copy(link from) {
	link into;
	if (from == 0)
		return 0;
	else if (from->tag == S) {
		into = new Node(from->car.a);
		into->couder = copy(from->couder);
	}
	else if (from->tag == I) {
		into = new Node(from->car.b);
		into->couder = copy(from->couder);
	}
	else if (from->tag == L) {
		into = new Node(copy(from->car.c));
		into->couder = copy(from->couder);
	}
	return into;
}

link cons (link p, link q) {
	link s, t;

	if (p == 0)
		cout << "\n cons: Missing first operand in cons \n";
	else if (p->couder != 0)
		cout << "\n cons: illegal sibling in the first argument\n";
	else
		s = copy(p);

	if (q == 0)
		cout << "\n cons: Missing second operand in cons \n";
	else if (q->couder != 0)
		cout << "\n cons: illegal sibling in the second argument\n";
	else if ((q->tag == S) && (strcmp(q->car.a->entry, "NIL         ") == 0))
		t = new Node();
	else if (q->tag != L)
		cout << "\n cons: Wrong second argument for cons\n";
	else
		t = copy(q);

	s->couder = t->car.c;
	t->car.c = s;
	t->couder = 0;
	return t;
}

link quote(char* name) {
	return (new Node(symbols.locate(name)));
}

link null(link p) {
	if (p==0 || 
		(p->tag == S && strcmp(p->car.a->entry, FALSE)==0))
		return (new Node(symbols.locate(TRUE)));
	else
		return (new Node(symbols.locate(FALSE)));
}

link atom(link p) {
	if (p == 0) {
		cout <<"\n atom: Missing operand in list\n";
		return 0;
	} else if (p==0 || p->tag == S || p->tag == I)
		return (new Node(symbols.locate(TRUE)));
	else
		return (new Node(symbols.locate(FALSE)));
}

link setq(char* name, link p) {
	if (p == 0)
		cout <<"\n setq: Missing second operand in setq \n";
	else {
		point t = symbols.locate(name);
		t->value = p;
	}
	return p;
}

link val(char* name) {
	return(symbols.locate(name)->value);
}

link list(link p) {
	link l = 0;
	if (p == 0)
		cout <<"\n list: missing operand in list\n";
	else if (p->tag == L)
		l = new Node(symbols.locate(TRUE));
	else
		l = new Node(symbols.locate(FALSE));
	return l;
}

link length(link p) {
	link l = 0;
	if (p == 0)
		cout <<"\n length: missing operand in list\n";
	else if (p->tag != L) {
		cout <<"\n length: operand must be a list\n";
	} else {
		int len = 0;
		link t;
		t = p->car.c;
		while (t != 0) {
			len++;
			t = t->couder;
		}
		l = new Node(len);
	}
	return l;
}

link cdr(link p) {
	link t = 0;
	if (p == 0)
		cout << " cdr: NIL pointer ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " cdr: operand must be a list\n";
	else {
		if (length(p)->car.b == 1)
			t = new Node(symbols.locate(FALSE));
		else {
			t = new Node(p->car.c->couder);
		}
	}
	return t;
}

link car(link p) {
	link t = 0;
	if (p == 0)
		cout << " car: NIL pointer ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " car: operand must be a list\n";
	else {
		if (p->car.c->tag == S)
			t = new Node(p->car.c->car.a);
		else if (p->car.c->tag == I)
			t = new Node(p->car.c->car.b);
		else
			t = new Node(p->car.c->car.c);
	}
	return t;
}

link append(link p, link q) {
	link t = 0;
	if (p == 0 || q == 0)
		cout << " append: NIL pointer(s) ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " append: first operand must be a list\n";
	else {
		link l = copy(p)->car.c;
		t = new Node(l);
		while (l->couder !=0)
			l = l->couder;
		link m;
		if (strcmp(list(q)->car.a->entry, TRUE) == 0)
			m = q->car.c;
		else
			m = q;
		l->couder = m;
	}
	return t;
}

link plus(link p) {
	link t = 0;
	if (p == 0)
		cout << " add: NIL pointer(s) ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " add: operand must be a flat list of numbers\n";
	else {
		link l = p->car.c;
		int sum = 0;
		while(l != 0) {
			if (l->tag != I) {
				cout << " add: non-Integer tag found\n";
				return 0;
			}
			sum += l->car.b;
			l = l->couder;
		}
		t = new Node(sum);
	}
	return t;
}

link minus(link p) {
	link t = 0;
	if (p == 0)
		cout << " minus: NIL pointer(s) ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " minus: operand must be a flat list of numbers\n";
	else {
		link l = p->car.c;
		int sum = 0;
		while(l != 0) {
			if (l->tag != I) {
				cout << " minus: non-Integer tag found\n";
				return 0;
			}
			sum -= l->car.b;
			l = l->couder;
		}
		t = new Node(sum);
	}
	return t;
}

link times(link p) {
	link t = 0;
	if (p == 0)
		cout << " times: NIL pointer(s) ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " times: operand must be a flat list of numbers\n";
	else {
		link l = p->car.c;
		int prod = 1;
		while(l != 0) {
			if (l->tag != I) {
				cout << " times: non-Integer tag found\n";
				return 0;
			}
			prod *= l->car.b;
			l = l->couder;
		}
		t = new Node(prod);
	}
	return t;
}

link max(link p) {
	link t = 0;
	if (p == 0)
		cout << " max: NIL pointer(s) ";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " max: operand must be a flat list of numbers\n";
	else {
		link l = p->car.c;
		int max = -999999;
		while(l != 0) {
			max = l->car.b > max ? l->car.b : max;
			l = l->couder;
		}
		t = new Node(max);
	}
	return t;
}

link min(link p) {
	link t = 0;
	if (p == 0)
		cout << " min: NIL pointer(s)\n";
	else if (strcmp(list(p)->car.a->entry, TRUE) != 0)
		cout << " min: operand must be a flat list of numbers\n";
	else {
		link l = p->car.c;
		int min = 999999;
		while(l != 0) {
			min = l->car.b < min ? l->car.b : min;
			l = l->couder;
		}
		t = new Node(min);
	}
	return t;
}

link number(link p) {
	link t = 0;
	if (p == 0)
		cout << " number: NIL pointer\n";
	else if (p->tag == I)
		t = new Node(symbols.locate(TRUE));
	else
		t = new Node(symbols.locate(FALSE));
	return t;
}

link add1(link p) {
	link t = 0;
	if (p == 0)
		cout << " add1: NIL pointer\n";
	else if (strcmp(number(p)->car.a->entry, TRUE) != 0)
		cout << " add1: operand must be a number\n";
	else {
		t = new Node(p->car.b + 1);
	}
	return t;
}

link sub1(link p) {
	link t = 0;
	if (p == 0)
		cout << " sub1: NIL pointer\n";
	else if (strcmp(number(p)->car.a->entry, TRUE) != 0)
		cout << " sub1: operand must be a number\n";
	else {
		t = new Node(p->car.b - 1);
	}
	return t;
}

link zero(link p) {
	link t = 0;
	if (p == 0)
		cout << " zero: NIL pointer\n";
	else if (strcmp(number(p)->car.a->entry, TRUE) != 0)
		cout << " zero: operand must be a number\n";
	else {
		if (p->car.b == 0)
			t = new Node(symbols.locate(TRUE));
		else
			t = new Node(symbols.locate(FALSE));
	}
	return t;
}

link equal(link p, link q) {
	link t = 0;
	if (p == 0 || q == 0)
		cout << " equal: NIL pointer(s) ";
	else if (strcmp(number(p)->car.a->entry, TRUE) != 0 ||
			 strcmp(number(q)->car.a->entry, TRUE) != 0)
		cout << " equal: both operands must be numbers\n";
	else {
		if (p->car.b == q->car.b)
			t = new Node(symbols.locate(TRUE));
		else
			t = new Node(symbols.locate(FALSE));
	}
	return t;
}

link lessp(link p, link q) {
	link t = 0;
	if (p == 0 || q == 0)
		cout << " lessp: NIL pointer(s) ";
	else if (strcmp(number(p)->car.a->entry, TRUE) != 0 ||
			 strcmp(number(q)->car.a->entry, TRUE) != 0)
		cout << " lessp: both operands must be numbers\n";
	else {
		if (p->car.b < q->car.b)
			t = new Node(symbols.locate(TRUE));
		else
			t = new Node(symbols.locate(FALSE));
	}
	return t;
}

link greaterp(link p, link q) {
	link t = 0;
	if (p == 0 || q == 0)
		cout << " greaterp: NIL pointer(s) ";
	else if (strcmp(number(p)->car.a->entry, TRUE) != 0 ||
			 strcmp(number(q)->car.a->entry, TRUE) != 0)
		cout << " greaterp: both operands must be numbers\n";
	else {
		if (p->car.b > q->car.b)
			t = new Node(symbols.locate(TRUE));
		else
			t = new Node(symbols.locate(FALSE));
	}
	return t;
}

link cond(link p, link q, link r) {
	link t = 0;
	if (p == 0 || q == 0 || r == 0)
		cout << " cond: NIL pointer(s) ";
	else if (strcmp(null(p)->car.a->entry, TRUE) != 0)
		t = copy(q);
	else
		t = copy(r);
	return t;
}

void main() {
//	void testRun();
//	testRun();

	char* F = "NIL         ";
	char* T = "T           ";
	char* U = "LISP        ";
	char* V = "PROLOG      ";
	char* W = "SURVEY      ";
	char* X = "POLYMORPHISM";
	char* Y = "INTERRUPTION";
	char* Z = "SIMULTANEOUS";
	point h,i, j, k, l, m, n, o;
	link f, t, p, q, r, s, u, v, x, y, z;

	h = symbols.locate(F);
	i = symbols.locate(T);
	j = symbols.locate(U);
	k = symbols.locate(V);
	l = symbols.locate(W);
	m = symbols.locate(X);
	n = symbols.locate(Y);
	o = symbols.locate(Z);

	f = new Node(h);
	t = new Node(i);
	p = new Node(j);
	q = new Node(k);
	r = new Node(l);
	s = new Node(m);
	u = new Node(n);
	v = new Node(o);

	cout << endl << "ADD1" << endl;
	x = new Node(500);
	y = add1(x);
	print(x); cout << endl;
	print(y); cout << endl;

	cout << endl << "SUB1" << endl;
	y = sub1(x);
	print(x); cout << endl;
	print(y); cout << endl;

	cout << endl << "APPEND" << endl;
	x = cons(p, f);
	x = cons(q, x);
	x = cons(r, x);
	y = cons(s, f);
	y = cons(u, y);
	y = cons(v, y);
	z = append(x, y);
	print(x); cout << endl;
	print(y); cout << endl;
	print(z); cout << endl;

	cout << endl<< "ATOM" << endl;
	x = cons(p, f);
	y = new Node(6);
	print(x); cout << endl;
	print(atom(x)); cout << endl;
	print(y); cout << endl;
	print(atom(y)); cout << endl;

	cout << endl<< "CAR" << endl;
	x = cons(s, f);
	x = cons(u, x);
	x = cons(v, x);
	z = car(x);
	print(x); cout << endl;
	print(z); cout << endl;
	cout << endl<< "CDR" << endl;
	z = cdr(x);
	print(x); cout << endl;
	print(z); cout << endl;

	cout << endl<< "COND" << endl;
	x = cond(t, p, q);
	print(t); print(p); print(q); cout << endl;
	print(x); cout << endl;
	x = cond(f, p, q);
	print(f); print(p); print(q); cout << endl;
	print(x); cout << endl;

	cout << endl << "CONS" << endl;
	x = cons(r, f);
	x = cons(s, x);
	y = cons(u, x);
	print(x); cout << endl;
	print(u); cout << endl;
	print(y); cout << endl;

	cout << endl << "COPY" << endl;
	y = copy(x);
	print(x); cout << endl;
	print(y); cout << endl;

	cout << endl << "EQUAL" << endl;
	x = new Node(250);
	y = new Node(250);
	z = new Node(500);
	print(x); cout << endl;
	print(y); cout << endl;
	print(equal(x, y)); cout << endl;
	print(x); cout << endl;
	print(z); cout << endl;
	print(equal(x, z)); cout << endl;

	cout << endl << "GREATERP" << endl;
	x = new Node(150);
	print(x); cout << endl;
	print(y); cout << endl;
	print(greaterp(x, y)); cout << endl;
	print(y); cout << endl;
	print(x); cout << endl;
	print(greaterp(y, x)); cout << endl;

	cout << endl << "LESSP" << endl;
	print(x); cout << endl;
	print(y); cout << endl;
	print(lessp(x, y)); cout << endl;
	print(y); cout << endl;
	print(x); cout << endl;
	print(lessp(y, x)); cout << endl;

	cout << endl << "LENGTH" << endl;
	x = cons(p, f);
	x = cons(q, x);
	x = cons(r, x);
	x = cons(s, x);
	x = cons(u, x);
	print(x); cout << endl;
	print(length(x)); cout << endl;

	cout << endl << "LIST" << endl;
	print(x); cout << endl;
	print(list(x)); cout << endl;
	print(v); cout << endl;
	print(list(v)); cout << endl;

	cout << endl << "MAX" << endl;
	x = cons(new Node(12), f);
	x = cons(new Node(-45), x);
	x = cons(new Node(75), x);
	x = cons(new Node(-17), x);
	x = cons(new Node(82), x);
	x = cons(new Node(33), x);
	print(x); cout << endl;
	print(max(x)); cout << endl;

	cout << endl << "MIN" << endl;
	print(x); cout << endl;
	print(min(x)); cout << endl;

	cout << endl << "PLUS" << endl;
	print(x); cout << endl;
	print(plus(x)); cout << endl;

	cout << endl << "MINUS" << endl;
	print(x); cout << endl;
	print(minus(x)); cout << endl;

	cout << endl << "TIMES" << endl;
	print(x); cout << endl;
	print(times(x)); cout << endl;

	cout << endl << "NULL" << endl;
	print(f); cout << endl;
	print(null(f)); cout << endl;
	x = cons(v, f);
	x = cons(u, x);
	print(x); cout << endl;
	print(null(x)); cout << endl;

	cout << endl << "NUMBER" << endl;
	x = s;
	y = new Node(300);
	print(x); cout << endl;
	print(number(x)); cout << endl;
	print(y); cout << endl;
	print(number(y)); cout << endl;

	cout << endl << "ZERO" << endl;
	x = new Node(300);
	y = new Node(0);
	print(x); cout << endl;
	print(zero(x)); cout << endl;
	print(y); cout << endl;
	print(zero(y)); cout << endl;

	cout << endl << "QUOTE" << endl;
	x = quote("Hello");
	print(x); cout << endl;

	cout << endl << "SETQ & VAL" << endl;
	x = setq("PI", new Node(317));
	cout << " PI = 317" << endl;
	print(val("PI")); cout << endl;

}

void testRun() {	
	symbols.dump();
	char* F = "NIL         ";
	char* T = "T           ";
	char* X = "POLYMORPHISM";
	char* Y = "INTERRUPTION";
	char* Z = "SIMULTANEOUS";
	point h,i, j, k, l;
	link f, t, p, q, r, s, u, v, z;

	h = symbols.locate(F);
	i = symbols.locate(T);
	j = symbols.locate(X);
	k = symbols.locate(Y);
	l = symbols.locate(Z);

	f = new Node(h);
	t = new Node(i);
	p = new Node(j);
	q = new Node(k);
	r = new Node(l);

	cout << "\n\n f -> ";
	print(f);
	cout << "\n\n t -> ";
	print(t);
	cout << "\n\n p -> ";
	print(p);
	cout << "\n\n q -> ";
	print(q);
	cout << "\n\n r -> ";
	print(r);

	s = cons(p, f);
	cout << "\n\n s -> ";
	print(s);
	u = cons(q, s);
	cout << "\n\n u -> ";
	print(u);
	v = cons(r, u);
	cout << "\n\n v -> ";
	print(v);
	z = copy(v);
	cout << "\n\n z -> ";
	print(z);

	p = new Node(1);
	q = new Node(2);
	r = new Node(3);

	s = cons(r, f);
	cout << "\n\n s -> ";
	print(s);
	s = cons(q, s);
	cout << "\n\n s -> ";
	print(s);
	s = cons(p, s);
	cout << "\n\n s -> ";
	print(s);
	z = cons(s, z);
	cout << "\n\n z -> ";
	print(z);
	z = cons(f, s);
	cout << "\n\n z -> ";
	print(z);
	h = symbols.locate("ARCHITECTURE");
	symbols.dump();
	s = new Node(h);
	cout << "\n\n s -> ";
	print(s);
	s = new Node();
	cout << "\n\n s -> ";
	print(s);
	v = cons(s, u);
	cout << "\n\n v -> ";
	print(v);
	cout << "\n";
}
