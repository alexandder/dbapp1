CREATE TRIGGER footballer_check 
BEFORE INSERT OR UPDATE ON footballer
       FOR EACH ROW EXECUTE PROCEDURE checkData();

CREATE OR REPLACE FUNCTION checkData()
RETURNS TRIGGER AS $$
BEGIN
	IF length(NEW.name) < 1 THEN
	   RAISE EXCEPTION 'Imie nie moze byc puste';
	END IF;
	IF length(NEW.lastname) < 1 THEN
	   RAISE EXCEPTION 'Nazwisko nie moze byc puste';
	END IF;
	IF NEW.salary < 0::money THEN
	   RAISE EXCEPTION 'Pensja nie moze byc ujemna';
	END IF;
	IF NEW.avaragenumberofgoals < 0 THEN
	   RAISE EXCEPTION 'Srednia liczba goli nie moze byc ujemna';
	END IF;
	IF NEW.numberofassists < 0 THEN
	   RAISE EXCEPTION 'Liczba asyst nie moze byc ujemna';
	END IF;
	NEW.name := initcap(NEW.name);
	NEW.lastname := replace(initcap(replace(NEW.lastname, '-', ' ')), ' ', '-');
	RETURN NEW;
END;
$$ LANGUAGE plpgsql;
