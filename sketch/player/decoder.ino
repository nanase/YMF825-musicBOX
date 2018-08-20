byte selx = SELX_LR_ENABLE;
extern byte sd_buffer[SD_BUFFER_SIZE];
extern volatile bool ymf825_playing;
extern volatile bool ymf825_next_file;

bool progress() {
  int16_t pos;

  if (!ymf825_playing)
    return true;

  if (ymf825_next_file) {
    ymf825_next_file = false;
    return false;
  }

  if ((pos = sd_read_buffer(1)) == EOF)
    return false;

  selx = sd_buffer[pos] & SELX_MASK;

  // Wait
  if (selx == SELX_WAIT) {
    wait_add((sd_buffer[pos] & LENX_MASK) + 1);
    return true;
  }

  // WOPx
  switch (sd_buffer[pos] & WOPX_MASK) {
    case WOPX_WRITE_DA_DD:
      if (!write_da_dd((sd_buffer[pos] & LENX_MASK) + 1))
        return false;
      break;

    case WOPX_WRITE_SA_DD:
      if (!write_sa_dd((sd_buffer[pos] & LENX_MASK) + 1))
        return false;
      break;
      
    case WOPX_BURSTWRITE_TONE:
      if (!burstwrite_tone(((int16_t)sd_buffer[pos] & LENX_MASK) + 1))
        return false;
      break;
      
    case WOPX_BURSTWRITE_EQ:
      if (!burstwrite_eq())
        return false;
      break;
  }

  return true;
}

bool write_da_dd(byte length) {
  int16_t pos;

  if ((pos = sd_read_buffer(length * 2)) == EOF)
    return false;

  for (byte i = 0; i < length; i++) {
    ymf825_select();
    ymf825_write(
      sd_buffer[pos + i * 2 + 0],   // address
      sd_buffer[pos + i * 2 + 1]    // data
    );
    ymf825_unselect();
  }

  return true;
}

bool write_sa_dd(byte length) {
  int16_t pos;

  if ((pos = sd_read_buffer(length + 1)) == EOF)
    return false;

  for (byte i = 0; i < length; i++) {
    ymf825_select();
    ymf825_write(
      sd_buffer[pos + 0],       // address
      sd_buffer[pos + i + 1]    // data
    );
    ymf825_unselect();
  }

  return true;
}

bool burstwrite_tone(int16_t length) {
  int16_t pos;

  if ((pos = sd_read_buffer(length * 30 + 6)) == EOF)
    return false;

  ymf825_select();
  ymf825_burstwrite(
    sd_buffer[pos + 0],
    &sd_buffer[pos + 1],
    length * 30 + 5
  );
  ymf825_unselect();

  return true;
}

bool burstwrite_eq() {
  int16_t pos;

  if ((pos = sd_read_buffer(16)) == EOF)
    return false;

  ymf825_select();
  ymf825_burstwrite(
    sd_buffer[pos + 0],
    &sd_buffer[pos + 1],
    15
  );
  ymf825_unselect();

  return true;
}
