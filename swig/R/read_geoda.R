
create_geoda_from_sf = function(obj) {
  print ("create geoda from sf object")
}

read_geoda = function(obj) {
  if (is(obj, "sf")) {
    create_geoda_from_sf(obj)
  } else if (typeof(obj) == "character") {
    return <- GeoDa(obj)
  } else {
    stop ("Only a sf object or a string of input datasource is allowed")
  }
}