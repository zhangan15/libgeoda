random_string <- function(n = 5000) {
  a <- do.call(paste0, replicate(5, sample(LETTERS, n, TRUE), FALSE))
  paste0(a, sprintf("%04d", sample(9999, n, TRUE)), sample(LETTERS, n, TRUE))
}

sf_to_geoda = function(sf_obj) {
  if (!requireNamespace("sf")) {
    stop("package sf not available: install first?")
  }
}

sp_to_geoda = function(sp_obj) {
  if (!requireNamespace("sp")) {
    stop("package sp not available: install first?")
  }
  # geometries
  geoms_wkb = writeWKB(sp_obj)
  n_obs <- length(geoms_wkb)
  wkb_bytes_len <- sapply(geoms_wkb, function(x) {return(length(x))})
  wkb_vec <- unlist(geoms_wkb)
  
  # in-memory name
  file_name <- random_string(1)
  
  # table
  col_names <- colnames(sp_obj@data)
  n_cols <- length(col_names)
  tbl <- GeoDaTable()
  for (i in 1:n_cols) {
    ft <- class(sp_obj@data[[i]])
    if (ft == "factor") {
      dat <- sp_obj@data[[i]]
      tbl$AddStringColumn(col_names[[i]], dat)

    } else if (ft == "integer" || ft == "logical") {
      dat <- sp_obj@data[[i]]
      tbl$AddIntColumn(col_names[[i]], dat)

    } else if (ft == "double" || ft == "numeric") {
      dat <- sp_obj@data[[i]]
      tbl$AddRealColumn(col_names[[i]], dat)

    } else {
      dat <- as.character(sp_obj@data[[i]])
      tbl$AddStringColumn(col_names[[i]], dat)
    }
  }
  # map_type
  map_type <- "map_polygons"
  if (is(sp_obj, 'SpatialPointsDataFrame')) {
    map_type <- "map_points"
  } else if (is(sp_obj, "SpatialLinesDataFrame-class")) {
    map_type <- "map_lines"
  }
  # prj4
  proj4_str <- sp_obj@proj4string
  gda <- GeoDa(file_name, map_type, n_obs, tbl, as.integer(wkb_vec), wkb_bytes_len, proj4_str@projargs)
  return(gda)
}

geoda_to_sp = function(gda) {
  # map_type
  map_type <- gda$GetMapType()
  if (map_type == "polygon_type") {
    # Creation of spatial polygons

  }
  # geometries
  wkb <- list()
  n_obs <- gda$GetNumObs()
  for (i in 1:n_obs) {
    append(wkb, as.raw(gda$GetGeometryWKB(i-1)))
  }
  obj <- readWKB(
    wkb,
    id = c("San Francisco", "New York"),
    proj4string = sp::CRS("+proj=longlat +ellps=WGS84 +datum=WGS84 +no_defs")
  )

  # use the WKB as a column in a data frame
  #ds <- data.frame(ID = c("a","b"), Geometry = wkb)
}