General information
---------------------------
This folder contains the script called `create_sql_queries_for_experimental_data` that can be used to import experimental data into a MySQL database.
The script uses an input card containing the basic information about the measurement to be inserted and the experimental data itself.
It produces a set of MySQL queries that act on `laboratory`, `collaboration`, `experiment`, `observable_kinematic` and `observable_result` tables of the database `partons`.
For the three first tables, it is checked if the new data do not double the existing entries (in the case of repetition, the existing entries are used). 

The script was written for and has been tested under GNU Awk 4.1.3.  

Structure of the input card
---------------------------
The input card should follow the following structure (check `CLAS_ALU_asymmetries.dat` for an example):

1) provide measurement information (keep information in quotation marks where indicated):

```
   LABORATORY     "name" 
   COLLABORATION  "name"
   EXPERIMENT     "year"   "process"   "type"   "reference"
```

2) provide experimental points (as many as you want):

```
   bin_index   xB    t     Q2    Ebeam    phi      observable_name   value     stat-     stat+      sys-      sys+      err_tot
```

If you want to use single quotation marks, you must escape it, _e.g._ `"e\' + p\' + gamma"` instead of `"e' + p' + gamma"`.

Usage
---------------------------
Run the script in the following way:

```py
   ./create_sql_queries_for_experimental_data your_input_card.dat
```

It will produce a file `output.sql` containing MySQL queries to be inserted into the `partons` database of your MySQL server.
To insert the file do like this:

```py
   # in shell, launch the MySQL client
   mysql -u your_sql_user -p
```

```sql
   /* in MySQL console */
   use partons;
   source output.sql;
```

If you are using the virtual machine provided by PARTONS team, set MySQL user to `partons` with the password `partons`.
