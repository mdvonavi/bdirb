CREATE TABLE IF NOT EXISTS pupils (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  Name STRING,
  CardNo INTGER,
  Birthday DATE,
  Sex CHAR NULL,
  Info STRING
);

CREATE TABLE IF NOT EXISTS typeMeasurement (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  name STRING,
  Comment STRING
);

CREATE TABLE IF NOT EXISTS Measurement (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  idTM INTEGER NOT NULL ,
  idPupils INTEGER NOT NULL,
  DateMeas INTEGER,
  Comment STRING,
  CONSTRAINT fkTM
    FOREIGN KEY (idTM)
    REFERENCES typeMeasurement (id)
    ON DELETE CASCADE,
  CONSTRAINT fkP
    FOREIGN KEY (idPupils)
    REFERENCES pupils (id)
    ON DELETE CASCADE
);

CREATE TABLE IF NOT EXISTS tm_params (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  idTM INTEGER NOT NULL ,
  name STRING NOT NULL,
  CONSTRAINT fkTM
    FOREIGN KEY (idTM)
    REFERENCES typeMeasurement (id)
    ON DELETE CASCADE
);


CREATE  TABLE IF NOT EXISTS m_unit (
  idM INTEGER NOT NULL,
  idTMP INTEGER NOT NULL,
  idTMC INTEGER NOT NULL,
  val STRING,
  CONSTRAINT fkM
    FOREIGN KEY (idM)
    REFERENCES Measurement (id)
    ON DELETE CASCADE,
  CONSTRAINT fkTMP
    FOREIGN KEY (idTMP)
    REFERENCES tm_params (id)
    ON DELETE CASCADE,
  CONSTRAINT fkTMP
    FOREIGN KEY (idTMC)
    REFERENCES tm_categories (id)
    ON DELETE CASCADE
    PRIMARY KEY  (idM, idTMP, idTMC)
);

CREATE TABLE IF NOT EXISTS tm_categories (
  id INTEGER PRIMARY KEY AUTOINCREMENT,
  idTM INTEGER NOT NULL,
  name STRING NOT NULL,
  CONSTRAINT fkTM
    FOREIGN KEY (idTM)
    REFERENCES typeMeasurement (id)
    ON DELETE CASCADE
);