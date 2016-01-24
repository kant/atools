drop table if exists airport;

create table airport
(
  airport_id integer primary key,
  file_id integer not null,
  ident text not null,
  region text,
  name text,
  country text not null,
  state text,
  city text not null,
  fuel_flags integer not null,
  num_helipads integer not null,
  has_avgas integer not null,
  has_jetfuel integer not null,
  has_boundary_fence integer not null,
  has_tower_object integer not null,
  has_taxiways integer not null,
  has_apron integer not null,
  has_jetways integer not null,
  mag_var real not null,
  tower_lonx real, 
  tower_laty real,
  altitude integer not null,
  lonx real not null, 
  laty real not null,
foreign key(file_id) references bgl_file(bgl_file_id)
);

drop table if exists com;

create table com
(
  com_id integer primary key,
  airport_id not null,
  type text not null,
  frequency integer not null,
  name text,
foreign key(airport_id) references airport(airport_id)
);
  
drop table if exists runway;

create table runway
(
  runway_id integer primary key,
  airport_id not null,
  primary_end_id integer not null,
  secondary_end_id integer  not null,
  surface text not null,
  length integer not null,
  width integer not null,
  heading real not null,
  pattern_altitude integer not null,
  marking_flags integer not null,
  light_flags integer not null,
  pattern_flags integer not null,
  edge_light text,
  center_light text,
  has_center_red integer not null,
  altitude integer not null,
  lonx real not null, 
  laty real not null,
foreign key(airport_id) references airport(airport_id),
foreign key(primary_end_id) references runway_end(runway_end_id),
foreign key(secondary_end_id) references runway_end(runway_end_id)
);

drop table if exists runway_end;

create table runway_end
(
  runway_end_id integer primary key,
  name text not null,
  offsetThreshold integer not null, 
  blastPad integer not null, 
  overrun integer not null,
  left_vasi_type text,
  left_vasi_pitch real,
  right_vasi_type text,
  right_vasi_pitch real,
  has_closed_markings integer not null,
  has_stol_markings integer not null,
  is_takeoff integer not null,
  is_landing integer not null,
  is_pattern text not null,
  app_light_system_type text,
  has_end_lights integer not null,
  has_reils integer not null,
  has_touchdown_lights integer not null,
  num_strobes integer not null
);
  
drop table if exists approach;

create table approach
(
  approach_id integer primary key,
  runway_end_id integer,
  type text not null,
  has_gps_overlay integer not null,
  num_legs integer not null,
  num_missed_legs integer not null,
  fix_type text not null,
  fix_ident text,
  fix_region text,
  fix_airport_ident text,
  altitude integer,
  heading real,
  missed_altitude integer,
foreign key(runway_end_id) references runway_end(runway_end_id)
);

drop table if exists transition;

create table transition
(
  transition_id integer primary key,
  approach_id integer not null,
  type text not null,
  num_legs integer not null,
  fix_type text not null,
  fix_ident text,
  fix_region text,
  fix_airport_ident text,
  altitude integer,
  dme_ident text,
  dme_region text,
  dme_airport_ident,
  dme_radial real,
  dme_distance integer,
foreign key(approach_id) references approach(approach_id)
);

drop table if exists parking;

create table parking
(
  parking_id integer primary key,
  airport_id integer not null,
  type text not null,
  name text not null,
  number integer not null,
  radius real not null,
  heading real not null,
  lonx real not null, 
  laty real not null,
foreign key(airport_id) references airport(airport_id)
);

drop table if exists delete_airport;

create table delete_airport
(
  delete_airport_id integer primary key,
  airport_id integer not null,
  approaches integer not null, 
  apronlights integer not null, 
  aprons  integer not null,
  frequencies  integer not null,
  helipads  integer not null,
  runways  integer not null,
  starts  integer not null,
  taxiways  integer not null,
foreign key(airport_id) references airport(airport_id)
);